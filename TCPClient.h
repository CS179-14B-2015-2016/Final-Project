#ifndef TCPCLIENT_H_INCLUDED
#define TCPCLIENT_H_INCLUDED

#include <boost/asio.hpp>
#include <map>
#include <thread>
#include <mutex>
#include "Message.h"
#include "GameState.h"
using namespace boost::asio;

struct client {
	ip::tcp::socket socket;
	ip::tcp::resolver resolver;
	std::string host, port;

	std::vector<uint8_t> recBuffer;
	int recTotal, recBytes;
	MessageType recType;

	std::vector<uint8_t> sendBuffer;

	GameState* gameState;
	std::string username;
	uint8_t** map;
	uint8_t ID;

	client(io_service* service, GameState* gameState);
	void connect(std::string host, std::string username);
	void receiveHeader();
	void receiveBody();
	void send();
};

#endif // TCPCLIENT_H_INCLUDED
