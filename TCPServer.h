#ifndef TCPSERVER_H_INCLUDED
#define TCPSERVER_H_INCLUDED

#include <boost/asio.hpp>
#include <map>
#include <thread>
#include <mutex>
#include "Message.h"
using namespace boost::asio;

struct server;

struct session {
	ip::tcp::socket socket;
	server* serverPtr;
	uint8_t id;

	std::vector<uint8_t> recBuffer;
	int recTotal, recBytes;
	MessageType recType;

	std::vector<uint8_t> sendBuffer;

	session(ip::tcp::socket socket, server* serverPtr, uint8_t id);
	ip::tcp::socket* getSocketPtr();
	void receiveHeader();
	void receiveBody();
	void setSendBuffer(const std::vector<uint8_t>& copy);
	void send();
};

struct server {
	ip::tcp::socket socket;
	ip::tcp::endpoint endpoint;
	ip::tcp::acceptor acceptor;

	uint8_t expectedClients;
	uint8_t totalClients;
	uint8_t mapLoadedClients;
	uint8_t monotonousCounter;
	std::vector<std::shared_ptr<session>> list;
	std::map<uint8_t, std::shared_ptr<session>> idList;
	std::map<std::string, std::shared_ptr<session>> nameList;
	std::vector<uint8_t> recBuffer;
	std::vector<uint8_t> sendBuffer;
	std::thread* receiveThread;
	std::thread* sendThread;
	std::mutex broadcastMtx;

	uint8_t** map;
	std::vector<uint8_t> compressedMap;
	std::vector<uint8_t> broadcastBuffer;
	bool inGame;

	server(io_service* service, uint8_t players);
	void accept();
	void setBroadcastBuffer(const std::vector<uint8_t>& copyBuffer);
	void broadcast();

	std::string correctUsername(const std::string& proposed);
	void generateMap();
	void compressMap();
	void sendMap();
	void update(float delta);
};

#endif // TCPSERVER_H_INCLUDED
