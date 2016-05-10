#pragma once

#include <vector>
#include <boost/asio.hpp>

#include "Message.h"
#include "Entity.h"
using namespace boost::asio;

class GameState;
struct ServerSession {
	std::vector<uint8_t> recBuffer;
	size_t recTotal;
	size_t recBytes;
	MessageType recType;
	std::string username;
	GameState* gamestate;
	std::shared_ptr<Entity> character;

	ip::tcp::socket socket;
	uint8_t id;
	bool isActive;

	ServerSession(ip::tcp::socket socket, uint8_t id, GameState* gamestate);
	ip::tcp::socket* getSocketPtr();
	void receiveHeader();
	void receiveBody();
	void interpretData();
	void activate(bool active);
	void send(const std::vector<uint8_t>& copy);
};