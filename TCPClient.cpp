#include "TCPClient.h"
#include "GameState.h"
#include "Message.h"
#include <string>

client::client(io_service* service, GameState* gameState) :
	socket(*service),
	resolver(*service),
	gameState(gameState)
{
	this->port = std::to_string(GameState::tcpPort);
}

void client::connect(std::string host, std::string username) {
	this->host = host;
	this->username = username;
	auto endpoint_iterator = resolver.resolve({ host, port });
	async_connect(socket, endpoint_iterator, [&](boost::system::error_code ec, ip::tcp::resolver::iterator it) {
		MessageHeader mh = MessageHeader{ MessageType::CONNECT, username.length()+1 };
		sendBuffer.clear();
		sendBuffer.resize( sizeof(MessageHeader) + username.length() + 1 );
		memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
		memcpy(sendBuffer.data()+sizeof(MessageHeader), username.data(), username.length()+1);
		send();
		receiveHeader();
	});
}

void client::receiveHeader() {
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

void client::receiveBody() {
	if (recBuffer.size() == 0)
		recBuffer.resize(recTotal);

	socket.async_receive(buffer(recBuffer.data() + recBytes, (recTotal - recBytes)), [&](boost::system::error_code ec, size_t size) {
		recBytes += size;
		if (recBytes < recTotal) {
			receiveBody();
		}
		else {
			switch (recType) {	
				case MessageType::CONFIRM: {
					std::string toRead = std::string(reinterpret_cast<char*>(recBuffer.data()));
					std::stringstream reader(toRead);
					std::string tempID; reader >> tempID;
					ID = std::atoi(tempID.c_str());
					reader >> username;

					MessageHeader ms = MessageHeader{ MessageType::MAP_REQ, 0 };
					sendBuffer.clear();
					sendBuffer.resize( sizeof(MessageHeader) );
					memcpy(sendBuffer.data(), &ms, sizeof(MessageHeader));
					send();
					break;
				}
				case MessageType::MAP: {
					map = new uint8_t*[25];
					for (int i = 0; i < 25; i++)
						map[i] = new uint8_t[32];
					for (int i = 0; i < 25; i++)
					{
						for (int j = 0; j < 32; j++)
						{
							map[i][j] = 1;
						}
					}

					int gems = (int) recBuffer[0];
					int gemOffset = 1;
					int rooms = (int) recBuffer[gemOffset + gems * 2];
					int roomOffset = gemOffset + gems * 2 + 1;

					for (int i = 0; i < gems * 2; gems += 2)
					{
						int xInd = gemOffset + i;
						int yInd = xInd+1;
						int x = (int) recBuffer[xInd];
						int y = (int) recBuffer[yInd];

						map[x][y] = 2;
					}

					for (int i = 0; i < rooms; i += 4)
					{
						int topInd = roomOffset + i;
						int leftInd = topInd + 1;
						int widthInd = leftInd + 1;
						int heightInd = widthInd + 1;

						int top = recBuffer[topInd];
						int left = recBuffer[leftInd];
						int width = recBuffer[widthInd];
						int height = recBuffer[heightInd];

						for (int j = top; j < top+height; j++)
						{
							for (int k = left; k < left + width; k++)
							{
								map[j][k] = 0;
							}
						}
					}

					break;
				}
			}

			recBuffer.clear();
			receiveHeader();
		}
	});
}

void client::send() {
	socket.async_send(buffer(sendBuffer.data(), sendBuffer.size()), [&](boost::system::error_code ec, size_t size) {
	});
}