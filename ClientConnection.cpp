#include "ClientConnection.h"
#include "GameState.h"

ClientConnection::ClientConnection(io_service& service, GameState* gamestate) :
	socket(service),
	resolver(service),
	gamestate(gamestate),
	isActive(true)
{
	socket.open(ip::tcp::v4());
	socket.set_option(ip::tcp::no_delay(true));
}

void ClientConnection::connect(std::string host, std::string username) {
	this->host = host;
	this->username = username;
	auto endpoint_iterator = resolver.resolve({this->host, std::to_string(GameState::tcpPort)});
	async_connect(socket, endpoint_iterator, [&](boost::system::error_code ec, ip::tcp::resolver::iterator it) {
		auto mh = MessageHeader{ MessageType::CONNECT, this->username.length()+1 };
		std::vector<uint8_t> sendBuffer;
		sendBuffer.resize( sizeof(MessageHeader) + mh.size );
		memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
		memcpy(sendBuffer.data() + sizeof(MessageHeader), this->username.data(), mh.size);
		send(sendBuffer);
		receiveHeader();
	});
}

void ClientConnection::receiveHeader() {
	if (recBuffer.size() == 0) {
		recTotal = sizeof(MessageHeader);
		recBytes = 0;
		recBuffer.resize(recTotal);
	}

	socket.async_receive(buffer(recBuffer.data() + recBytes, (recTotal - recBytes)), [&](boost::system::error_code ec, size_t size) {
		recBytes += size;
		if (recBytes < recTotal) {
			receiveHeader();
		}
		else {
			MessageHeader* header = reinterpret_cast<MessageHeader*>(recBuffer.data());
			recBytes = 0;
			recTotal = header->size;
			recType = header->type;
			recBuffer.clear();
			receiveBody();
		}
	});
}

void ClientConnection::receiveBody() {
	if (recBuffer.size() == 0)
		recBuffer.resize(recTotal);

	socket.async_receive(buffer(recBuffer.data() + recBytes, (recTotal - recBytes)), [&](boost::system::error_code ec, size_t size) {
		recBytes += size;
		if (recBytes < recTotal)
		{
			receiveBody();
		}
		else
		{
			interpretData();
		}
	});
}

void ClientConnection::send(const std::vector<uint8_t>& copy) {
	auto sendBuffer = std::make_shared<std::vector<uint8_t>>(copy);
	socket.async_send(buffer(sendBuffer->data(), sendBuffer->size()), [sendBuffer](boost::system::error_code ec, size_t size) {
	});
}

void ClientConnection::interpretData() {
	switch (recType)
	{
		case MessageType::CONFIRM: {
			std::string content = std::string(reinterpret_cast<char*>(recBuffer.data()));
			std::stringstream ss(content);
			std::string idSubstring;
			ss >> idSubstring;
			id = std::atoi(idSubstring.c_str());
			gamestate->yourID = id;

			username = "";
			std::string temp;
			while (ss >> temp)
				username += temp + " ";
			if (username.length() > 0)
				username = username.substr(0, username.length() - 1);

			std::string toSend = "";
			auto mh = MessageHeader{ MessageType::MAP_REQUEST, toSend.length() + 1 };
			std::vector<uint8_t> sendBuffer;
			sendBuffer.resize(sizeof(MessageHeader) + mh.size);
			memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
			memcpy(sendBuffer.data() + sizeof(MessageHeader), toSend.data(), mh.size);
			send(sendBuffer);

			break;
		}
		case MessageType::MAP_DATA: {
			auto recdata = reinterpret_cast<MapMessage*>(recBuffer.data());
			auto linearized = std::string(reinterpret_cast<char*>(recBuffer.data() + sizeof(MapMessage)));
			gamestate->totalClients = recdata->clients;

			gamestate->mapData = new uint8_t*[ recdata->height ];
			for (auto i = 0; i < recdata->height; i++)
			{
				gamestate->mapData[i] = new uint8_t[ recdata->width ];
				for (auto j = 0; j < recdata->width; j++) {
					auto position = i*recdata->width + j;
					gamestate->mapData[i][j] = (uint8_t)(linearized[position] - '0');
				}
			}
			gamestate->map = new Map(gamestate->mapData, recdata->height, recdata->width);
			gamestate->compressMap();

			gamestate->entities = new std::vector<std::shared_ptr<Entity>>();
			gamestate->entities->resize( Universal::ENTITY_COUNT );
			for (auto i = 0; i < Universal::ENTITY_COUNT; i++)
				(*gamestate->entities)[i] = std::make_shared<Entity>(0, 0, gamestate, false, i);

			gamestate->character = (*gamestate->entities)[id];

			std::string toSend = "";
			auto mh = MessageHeader{ MessageType::READY, toSend.length() + 1 };
			std::vector<uint8_t> sendBuffer;
			sendBuffer.resize(sizeof(MessageHeader) + mh.size);
			memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
			memcpy(sendBuffer.data() + sizeof(MessageHeader), toSend.data(), mh.size);
			send(sendBuffer);

			break;
		}
		case MessageType::GAME_START: {
			gamestate->inGame = true;

			break;
		}
		case MessageType::UPDATE_DATA: {
			auto data = reinterpret_cast<UpdateDataMessage*>(recBuffer.data());
			(*gamestate->entities)[data->ID]->update(*data);
			break;
		}
		case MessageType::JEWEL_TAKEN: {
			auto jewel = reinterpret_cast<JewelTakenMessage*>(recBuffer.data());
			gamestate->map->getTile(jewel->i, jewel->j)->setType(0);
			break;
		}
		case MessageType::GAME_FINISH: {
			isActive = false;
			gamestate->inGame = false;
			gamestate->isFinished = true;

			std::string toSend = "";
			auto mh = MessageHeader{ MessageType::DISCONNECT, toSend.length() + 1 };
			std::vector<uint8_t> sendBuffer;
			sendBuffer.resize(sizeof(MessageHeader) + mh.size);
			memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
			memcpy(sendBuffer.data() + sizeof(MessageHeader), toSend.data(), mh.size);
			send(sendBuffer);

			break;
		}
	}

	recBuffer.clear();
	if (isActive)
		receiveHeader();
}