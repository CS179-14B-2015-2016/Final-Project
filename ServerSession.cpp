#include "ServerSession.h"
#include "GameState.h"

using namespace boost::asio;

ServerSession::ServerSession(ip::tcp::socket socket, uint8_t id, GameState* gamestate) :
	socket(std::move(socket)),
	gamestate(gamestate),
	recBytes(0),
	recTotal(0),
	isActive(true),
	id(id)
{
	socket.set_option(ip::tcp::no_delay(true));
	receiveHeader();
}

ip::tcp::socket* ServerSession::getSocketPtr() {
	return &socket;
}

void ServerSession::receiveHeader() {
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

void ServerSession::receiveBody() {
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

void ServerSession::interpretData() {
	switch (recType)
	{
		case MessageType::CONNECT: {

			std::string desiredName = std::string(reinterpret_cast<char*>(recBuffer.data()));
			desiredName = gamestate->checkName(desiredName);
			gamestate->registerName(desiredName);

			std::string toSend = std::to_string(id) + " " + desiredName;
			MessageHeader mh = MessageHeader{ MessageType::CONFIRM, toSend.length()+1 };
			std::vector<uint8_t> sendBuffer;
			sendBuffer.resize( sizeof(MessageHeader) + mh.size );
			memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
			memcpy(sendBuffer.data()+sizeof(MessageHeader), toSend.data(), mh.size);
			send(sendBuffer);
			break;
		}
		case MessageType::MAP_REQUEST: {

			MapMessage ms = MapMessage{ gamestate->totalClients, gamestate->map->getHeight(), gamestate->map->getWidth()};
			MessageHeader mh = MessageHeader{ MessageType::MAP_DATA, sizeof(MapMessage) + gamestate->compressedMap.length()+1 };
			std::vector<uint8_t> sendBuffer;
			sendBuffer.resize(sizeof(MessageHeader) + mh.size);
			memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
			memcpy(sendBuffer.data() + sizeof(MessageHeader), &ms, sizeof(MapMessage));
			memcpy(sendBuffer.data() + sizeof(MessageHeader) + sizeof(MapMessage), gamestate->compressedMap.data(), gamestate->compressedMap.length()+1);
			send(sendBuffer);
			break;
		}
		case MessageType::READY: {
			gamestate->readyClients++;

			auto pos = gamestate->findFreePosition();
			gamestate->reserveMap(pos.x, pos.y);
			character = std::make_shared<Entity>(pos.x, pos.y, gamestate, true, id);
			(*gamestate->entities)[id] = character;

			if (gamestate->readyClients == gamestate->totalClients)
			{
				std::string toSend = "";
				MessageHeader mh = MessageHeader{ MessageType::GAME_START, toSend.length() + 1 };
				std::vector<uint8_t> sendBuffer;
				sendBuffer.resize(sizeof(MessageHeader) + mh.size);
				memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
				memcpy(sendBuffer.data() + sizeof(MessageHeader), toSend.data(), mh.size);
				gamestate->broadcast(sendBuffer);

				gamestate->inGame = true;
			}
			break;
		}
		case MessageType::UPDATE_INPUT: {
			auto inputs = reinterpret_cast<UpdateInputMessage*>(recBuffer.data());
			
			gamestate->mapMtx.lock();
			gamestate->dataMtx.lock();
			(*gamestate->entities)[id]->handleInput(*inputs);
			gamestate->dataMtx.unlock();
			gamestate->mapMtx.unlock();
			
			break;
		}
		case MessageType::DISCONNECT: {
			gamestate->stillConnected--;

			if (gamestate->stillConnected == 1) {
				(*gamestate->sessions)[this->id] = nullptr;
				isActive = false;
			}
		}
	}
	if (isActive) {
		recBuffer.clear();
		receiveHeader();
	}
}

void ServerSession::activate(bool active) {
	isActive = active;
}

void ServerSession::send(const std::vector<uint8_t>& copy) {
	auto sendBuffer = std::make_shared<std::vector<uint8_t>>(copy);
	socket.async_send(buffer(sendBuffer->data(), sendBuffer->size()), [sendBuffer](boost::system::error_code ec, size_t size) {
	});
}