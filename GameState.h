#ifndef GAMESTATE_H_INCLUDED
#define GAMESTATE_H_INCLUDED
#include <thread>
#include <boost/asio.hpp>
#include <set>
#include <string>
#include <mutex>
#include "State.h"
#include "map.h"
#include <vector>
#include <queue>
#include "mapgenerator.h"
#include "ServerSession.h"
#include "ClientConnection.h"
#include "Entity.h"

using namespace boost::asio;

class GameState: public State {
private:
    Map* map;
    StateManager* sm;
	io_service service;
    bool isActive;
    bool isHost;
	bool inGame;
	bool isFinished;
	std::thread* networkThread;
	std::thread* service_runner;
	std::mutex dataMtx;
	std::mutex mapMtx;

	int KEY_UP = 22;//W
	int KEY_DOWN = 18;//S
	int KEY_LEFT = 0;//A
	int KEY_RIGHT = 3;//D
	int KEY_ACTION = 57; // SPACE
	int KEY_SHIFT = 38; // LSHIFT

	// SERVER STUFF
	friend struct ServerSession;

	MapGenerator* generator; //new
	std::string compressedMap;
	std::vector<std::shared_ptr<ServerSession>>* sessions;
	std::set<std::string>* nameList;
	std::vector<std::shared_ptr<Entity>>* entities;
	std::shared_ptr<Entity> character;
	uint8_t** mapData;
	size_t totalClients;
	size_t vacantPlayers;
	size_t readyClients;
	size_t stillConnected;

	ip::tcp::endpoint* endpoint;
	ip::tcp::acceptor* acceptor;
	ip::tcp::socket* socket;
	void accept();
	void broadcast(const std::vector<uint8_t>& copy);
	std::string checkName(const std::string& desired);
	void registerName(std::string username);
	void readMapFromFile();
	void positionEntities();
	sf::Vector2i findFreePosition();

	// CLIENT STUFF
	friend struct ClientConnection;
	ClientConnection* client;

	void readKeyBindings();
public:
	static const uint16_t tcpPort = 8080;

    GameState(StateManager* sm);
    void serverLoop(bool generateMap, size_t players, std::string username);
    void clientLoop(std::string ip, std::string username);
    void onActivate(const std::string& accept);
    void onDeactivate();
    void handleInput(int u, int v, const std::string& typed, sf::Event e);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;

	bool isMapFree(int i, int j);
	void reserveMap(int i, int j);
	void freeMap(int i, int j);
};

#endif // GAMESTATE_H_INCLUDED
