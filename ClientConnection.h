#pragma once

#include <vector>
#include <boost/asio.hpp>
#include "Message.h"

using namespace boost::asio;

class GameState;
struct ClientConnection {
	ip::tcp::socket socket;
	ip::tcp::resolver resolver;
	std::string host, username;
	GameState* gamestate;
	uint8_t id;
	bool isActive;

	std::vector<uint8_t> recBuffer;
	size_t recTotal;
	size_t recBytes;
	MessageType recType;

	ClientConnection(io_service& service, GameState* gamestate);
	void connect(std::string host, std::string username);
	void receiveHeader();
	void receiveBody();
	void interpretData();
	void send(const std::vector<uint8_t>& copy);
};