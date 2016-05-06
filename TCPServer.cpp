#include "TCPServer.h"
#include "GameState.h"
#include "Message.h"
#include <string>

// SESSION

session::session(ip::tcp::socket socket, server* serverPtr, uint8_t id) :
	socket(std::move(socket)),
	serverPtr(serverPtr),
	id(id),
	recTotal(0), 
	recBytes(0) 
{
}

ip::tcp::socket* session::getSocketPtr() {
	return &socket;
}

void session::receiveHeader() {
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

void session::receiveBody() {
	if (recBuffer.size() == 0)
		recBuffer.resize(recTotal);

	socket.async_receive(buffer(recBuffer.data() + recBytes, (recTotal - recBytes)), [&](boost::system::error_code ec, size_t size) {
		recBytes += size;
		if (recBytes < recTotal) {
			receiveBody();
		}
		else {
			switch (recType)
			{
				case MessageType::CONNECT: {
					std::string username = std::string(reinterpret_cast<char*>(recBuffer.data()));
					username = serverPtr->correctUsername(username);
					serverPtr->nameList[username] = serverPtr->idList[id];

					std::string toSend = std::to_string((int)id) + " " + username;
					MessageHeader mh = MessageHeader{ MessageType::CONFIRM, toSend.length()+1 };

					sendBuffer.clear();
					sendBuffer.resize( sizeof(MessageHeader) + toSend.length() + 1 );
					memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
					memcpy(sendBuffer.data()+sizeof(MessageHeader), toSend.data(), toSend.length()+1);
					send();
					break;
				}
				case MessageType::MAP_REQ: {
					std::vector<uint8_t> prepare;
					prepare.resize(sizeof(MessageHeader) + serverPtr->compressedMap.size());
					MessageHeader toSend = MessageHeader{ MessageType::MAP, serverPtr->compressedMap.size() };

					memcpy(prepare.data(), &toSend, sizeof(MessageHeader));
					memcpy(prepare.data() + sizeof(MessageHeader), serverPtr->compressedMap.data(), serverPtr->compressedMap.size());
					send();
					break;
				}
				case MessageType::WAIT: {
					break;
				}
				case MessageType::POSITION: {
					break;
				}
			}

			recBuffer.clear();
			receiveHeader();
		}
	});
}

void session::setSendBuffer(const std::vector<uint8_t>& copy) {
	sendBuffer.clear();
	sendBuffer = std::vector<uint8_t>(copy);
}

void session::send() {
	socket.async_send(buffer(sendBuffer.data(), sendBuffer.size()), [&](boost::system::error_code ec, size_t size) {
	});
}

// SERVER

server::server(io_service* service, uint8_t players) :
	socket(*service),
	endpoint(ip::tcp::v4(), GameState::tcpPort),
	acceptor(*service, endpoint),
	expectedClients(players),
	totalClients(0),
	monotonousCounter(0)
{
	generateMap();
	compressMap();
	accept();
}

void server::accept() {
	acceptor.async_accept(socket, [&](boost::system::error_code ec) {
		list.push_back(std::make_shared<session>(std::move(socket), this, monotonousCounter));
		idList[monotonousCounter++] = list.back();

		list.back()->receiveHeader();
		totalClients++;
		if (totalClients < expectedClients)
			accept();
		else
			sendMap();
	});
}

void server::setBroadcastBuffer(const std::vector<uint8_t>& copyBuffer) {
	broadcastMtx.lock();
	broadcastBuffer.clear();
	broadcastBuffer = std::vector<uint8_t>(copyBuffer);
	broadcastMtx.unlock();
}

void server::broadcast() {
	broadcastMtx.lock();
	for (int i=0; i<list.size(); i++)
	{
		list[i]->setSendBuffer(broadcastBuffer);
	}
	broadcastMtx.unlock();
}

std::string server::correctUsername(const std::string& proposed)
{
	std::string curr = proposed;
	while (nameList.find(curr) == nameList.end()) {
		auto length = curr.length();
		auto firstIndex = curr.length();

		for (int i = curr.length() - 1; i >= 0; i--)
		{
			if ('0' <= curr[i] && curr[i] <= '9')
				firstIndex--;
			else
				break;
		}

		if (firstIndex == curr.length())
			curr += "0";
		else {
			auto substringA = curr.substr(0, firstIndex);
			auto substringB = curr.substr(firstIndex, length - firstIndex);
			auto currCount = std::atoi(substringB.c_str());
			curr = substringA + std::to_string( currCount + 1 );
		}
	}
	return curr;
}

void server::generateMap() {
	map = new uint8_t*[25];
	for (uint8_t i = 0; i < 25; i++)
	{
		map[i] = new uint8_t[32];
	}

	// REPLACE WITH ACTUAL CONTENT --------
	for (uint8_t i = 0; i < 25; i++)
		for (uint8_t j = 0; j < 32; j++)
			map[i][j] = (i % 2);
	// ------------------------------------
}

void server::compressMap() {
	compressedMap.clear();
	compressedMap.push_back(0);

	std::vector<uint8_t> rectPos;
	rectPos.push_back(0);

	for (uint8_t i = 0; i<25; i++)
	{
		for (uint8_t j = 0; j<32; j++)
		{
			if (map[i][j] != 1) // NOT UNPASSABLE
			{
				if (map[i][j] == 2) // GEM
				{
					compressedMap.push_back(i);
					compressedMap.push_back(j);
					compressedMap[0]++;
				}

				uint8_t top = (uint8_t)i;
				uint8_t left = (uint8_t)j;

				uint8_t width = 1;
				uint8_t height = 1;
				for (int k = j + 1; k<32; k++)
				{
					if (map[i][k] == 1)
						break;
					if (map[i][k] == 2)
					{
						compressedMap.push_back(i);
						compressedMap.push_back(k);
						compressedMap[0]++;
					}
					map[i][k] = 1;
					width++;
				}

				for (int k = i + 1; k<25; k++)
				{
					bool addNew = true;
					for (int l = 0; l<width && addNew; l++)
					{
						if (map[k][left + l] == 1) {
							addNew = false;
						}
						if (map[k][left + l] == 2)
						{
							compressedMap.push_back(k);
							compressedMap.push_back(left + l);
							compressedMap[0]++;
						}
					}
					if (!addNew)
						break;

					for (int l = 0; l<width; l++)
						map[k][left + l] = 1;
					height++;
				}

				rectPos.push_back(top);
				rectPos.push_back(left);
				rectPos.push_back(width);
				rectPos.push_back(height);
				rectPos[0]++;
			}
		}
	}

	for (int i = 0; i < rectPos.size(); i++)
		compressedMap.push_back( rectPos[i] );
}

void server::sendMap() {
	std::vector<uint8_t> prepare;
	prepare.resize(sizeof(MessageHeader) + compressedMap.size());
	MessageHeader toSend = MessageHeader{MessageType::MAP, compressedMap.size()};

	memcpy(prepare.data(), &toSend, sizeof(MessageHeader));
	memcpy(prepare.data()+sizeof(MessageHeader), compressedMap.data(), compressedMap.size());
	
	setBroadcastBuffer(prepare);
	broadcast();
}

void server::update(float delta) {
	// TODO
}