#include "StateManager.h"
#include "GameState.h"
#include <sstream>
#include <cassert>
#include <fstream>
#include "Universal.h"
#include "ServerSession.h"

//using namespace boost::asio;

GameState::GameState(StateManager* sm): State(sm), sm(sm){
	
}

void GameState::serverLoop(bool generateMap, size_t player, std::string username) {
	yourID = 0;
	readyClients = 1;
	totalClients = stillConnected = player;

	if (generateMap)
		map = generator->generate(32, 25, 2);
	else
		readMapFromFile();

	mapData = map->printTiles();

	auto pos = findFreePosition();

	compressMap();

	nameList = new std::set<std::string>();
	endpoint = new ip::tcp::endpoint(ip::tcp::v4(), GameState::tcpPort);
	acceptor = new ip::tcp::acceptor(service, *endpoint);
	socket = new ip::tcp::socket(service);
	sessions = new std::vector<std::shared_ptr<ServerSession>>();
	entities = new std::vector<std::shared_ptr<Entity>>();
	entities->resize( Universal::ENTITY_COUNT );
	
	{
		auto position = findFreePosition();
		reserveMap(position.x, position.y);
		character = std::make_shared<Entity>(position.x, position.y, this, true, 0);
		(*entities)[0] = character;
	}

	for (auto i = player; i < Universal::ENTITY_COUNT; i++)
	{
		auto position = findFreePosition();
		reserveMap(position.x, position.y);
		(*entities)[i] = std::make_shared<Entity>(position.x, position.y, this, false, i);
	}

	sessions->push_back(nullptr);
	registerName(username);

	vacantPlayers = player - 1;
	if (vacantPlayers == 0)
		inGame = true;

	accept();

	service.run();
}

void GameState::accept() {
	if (vacantPlayers == 0)
		return;
	
	acceptor->async_accept(*socket, [&](boost::system::error_code ec) {
		sessions->push_back(std::make_shared<ServerSession>(std::move(*socket), sessions->size(), this));
		vacantPlayers--;
		accept();
	});
}

void GameState::broadcast(const std::vector<uint8_t>& copy) {
	for (auto session : *sessions) {
		if(session!=nullptr)
			session->send(copy);
	}
}

std::string GameState::checkName(const std::string& desired) {

	std::string curr = desired;
	while (nameList->find(curr) != nameList->end()) {
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
			curr = substringA + std::to_string(currCount + 1);
		}
	}
	return curr;
}

void GameState::registerName(std::string username) {
	(*nameList).insert(username);
}

void GameState::clientLoop(std::string IP, std::string username) {
	client = new ClientConnection(service, this);
	client->connect(IP, username);
	service.run();
}

void GameState::onActivate(const std::string& activate) {
	isActive = true;
	inGame = false;
	isFinished = false;

	readKeyBindings();
	
	std::stringstream ss(activate);
	std::string current;

	ss >> current;
	if (current == "host")
	{
		generator = new MapGenerator();

		isHost = true;
		ss >> current;
		bool toGenerate = (current == "Generate");

		ss >> current;
		size_t players = std::atoi(current.c_str());

		std::string username = "";
		while (ss >> current)
			username += current + " ";
		if (username.length() > 0)
			username = username.substr(0, username.length()-1);

		networkThread = new std::thread(&GameState::serverLoop, this, toGenerate, players, username);
	}
	else
	{
		isHost = false;
		std::string ipAddress;
		ss >> ipAddress;

		std::string username = "";
		while (ss >> current)
			username += current + " ";
		if (username.length() > 0)
			username = username.substr(0, username.length() - 1);

		networkThread = new std::thread(&GameState::clientLoop, this, ipAddress, username);
	}
}

void GameState::compressMap() {
	compressedMap = "";
	for (int i = 0; i < map->getHeight(); i++)
	{
		std::string line = "";
		for (int j = 0; j < map->getWidth(); j++)
		{
			line += std::to_string((unsigned int)mapData[i][j]);
		}
		compressedMap += line;
	}
}

void GameState::readKeyBindings() {
	std::ifstream configfile;
	configfile.open("config.txt");
	std::string buffer;
	int binding = 0;
	while (!configfile.eof())
	{
		buffer = "";
		configfile >> buffer;

		switch (binding)
		{
			case 0:
			{
				KEY_UP = stoi(buffer);
				break;
			}
			case 1:
			{
				KEY_DOWN = stoi(buffer);
				break;
			}
			case 2:
			{
				KEY_LEFT = stoi(buffer);
				break;
			}
			case 3:
			{
				KEY_RIGHT = stoi(buffer);
				break;
			}
			case 4:
			{
				KEY_ACTION = stoi(buffer);
				break;
			}
			case 5:
			{
				KEY_SHIFT = stoi(buffer);
				break;
			}

		}

		binding++;
	}

	configfile.close();
}

void GameState::readMapFromFile() {
	std::ifstream openmap;
	openmap.open("savedmap.txt");

	size_t height = 25;
	size_t width = 32;

	mapData = new uint8_t*[height];
	for (uint8_t i = 0; i<height; i++)
	{
		mapData[i] = new uint8_t[width];
	}

	for (int j = 0; j<height; j++)
	{

		std::string buffer;
		openmap >> buffer;

		for (int i = 0; i < width; i++)
		{
			char x = buffer[i];
			int ix = x - '0';
			if (ix == 4)
				ix = 2;
			mapData[j][i] = ix;
		}
	}

	openmap.close();
	map = new Map(mapData, height, width);
}

void GameState::positionEntities() {

	for (auto i = 0; i < totalClients; i++)
	{
		auto position = findFreePosition();
		mapData[position.x][position.y] = 9;
	}

	auto freeCounter = 0;

	for (auto i = 0; i < map->getHeight(); i++)
	{
		for (auto j = 0; j < map->getWidth(); j++)
		{
			auto test = mapData[i][j];
			if (test == 0 || test == 3)
			{
				freeCounter++;
			}
		}
	}

	auto totalNPC = (freeCounter / 8) - totalClients;

	for (auto i = 0; i < totalNPC; i++)
	{
		auto position = findFreePosition();
		mapData[position.x][position.y] = 8;
	}

}

sf::Vector2i GameState::findFreePosition() {
	mapMtx.lock();
	while (true)
	{
		auto xPos = rand() % map->getHeight();
		auto yPos = rand() % map->getWidth();
		auto test = sf::Vector2i(xPos, yPos);
		
		auto position = 0;

		if (test.x == 0 || (mapData[test.x - 1][test.y] == 0 || mapData[test.x - 1][test.y] == 3))
			position++;
		if (test.x == map->getHeight() - 1 || (mapData[test.x + 1][test.y] == 0 || mapData[test.x + 1][test.y] == 3))
			position++;
		if (test.y == 0 || (mapData[test.x][test.y - 1] == 0 || mapData[test.x][test.y - 1] == 3))
			position++;
		if (test.y == map->getWidth() - 1 || (mapData[test.x][test.y + 1] == 0 || mapData[test.x][test.y + 1] == 3))
			position++;
		
		if ((mapData[test.x][test.y] == 0 || mapData[test.x][test.y] == 3) && position > 2) {
			mapMtx.unlock();
			return test;
		}
	}
}

void GameState::onDeactivate() {
    isActive = false;
}

void GameState::handleInput(int u, int v, const std::string& typed, sf::Event e) {
	if (inGame) {
		auto inputs = UpdateInputMessage{ 0,
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key(KEY_UP)),
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key(KEY_DOWN)),
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key(KEY_LEFT)),
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key(KEY_RIGHT)),
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key(KEY_ACTION)),
			sf::Keyboard::isKeyPressed(sf::Keyboard::Key(KEY_SHIFT)) };

		if (isHost) {
			character->handleInput(inputs);
		}
		else {
			inputs.ID = client->id;

			auto mh = MessageHeader{ MessageType::UPDATE_INPUT, sizeof(UpdateInputMessage) };
			std::vector<uint8_t> sendBuffer;
			sendBuffer.resize(sizeof(MessageHeader) + mh.size);
			memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
			memcpy(sendBuffer.data() + sizeof(MessageHeader), &inputs, mh.size);
			client->send(sendBuffer);
		}
	}
	else if (isFinished) {

	}
	
}

void GameState::update(float dt) {
	if (inGame) 
	{
		hud.update(dt);
		if (isHost)
		{
			if (hud.update(0))
			{
				inGame = false;
				isFinished = true;

				std::string toSend = "";
				MessageHeader mh = MessageHeader{ MessageType::GAME_FINISH, toSend.length() + 1 };
				std::vector<uint8_t> sendBuffer;
				sendBuffer.resize(sizeof(MessageHeader) + mh.size);
				memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
				memcpy(sendBuffer.data() + sizeof(MessageHeader), toSend.data(), mh.size);
				broadcast(sendBuffer);

				for (auto session : *sessions) {
					if (session != nullptr)
						session->activate(false);
				}
			}
			else
			{
				dataMtx.lock();
				for (auto entity : *entities) {
					entity->update(dt);
					auto toSend = entity->getData();
					auto mh = MessageHeader{ MessageType::UPDATE_DATA, sizeof(UpdateDataMessage) };
					std::vector<uint8_t> sendBuffer;
					sendBuffer.resize(sizeof(MessageHeader) + mh.size);
					memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
					memcpy(sendBuffer.data() + sizeof(MessageHeader), &toSend, mh.size);
					broadcast(sendBuffer);
				}
				dataMtx.unlock();
			}
		}
	}
	else if(isFinished) {
		sm->push(5, compressedMap);
	} 
}

void GameState::draw(sf::RenderWindow& window) const {
	if (inGame) {
		sf::View minimap(sf::FloatRect(0,0, window.getSize().x, window.getSize().y));
		minimap.setViewport(sf::FloatRect(0.75f, 0, 0.25f, 0.25f));
		map->drawTiles(&window);

		for (auto entity : *entities)
			if (entity != character)
				entity->draw(window);

		window.setView(minimap);
		map->drawTiles(&window);
		character->draw(window, false);
		window.setView(window.getDefaultView());
		hud.draw(window);
		character->draw(window, true);
	}
}

bool GameState::isMapFree(int i, int j) {
	if (i < 0 || i >= map->getHeight() || j < 0 || j >= map->getWidth())
		return false;
	return mapData[i][j] == 0 || mapData[i][j] == 3;
}

void GameState::reserveMap(int i, int j, uint8_t replacement) {
	mapData[i][j] = replacement;
}

void GameState::freeMap(int i, int j) {
	mapData[i][j] = 0;
}

uint8_t GameState::getEntityCode(int i, int j) {
	return mapData[i][j];
}

std::shared_ptr<Entity> GameState::getEntityAt(int i, int j) {
	for (auto entity : *entities)
	{
		auto pos = entity->getPosition();
		if (pos.x == i && pos.y == j)
			return entity;
	}
	return nullptr;
}

void GameState::takeJewel(int i, int j) {
	assert(mapData[i][j] == 2);
	mapData[i][j] = 0;
	map->getTile(i, j)->setType(0);

	auto toSend = JewelTakenMessage{ (uint8_t)i, (uint8_t)j };
	auto mh = MessageHeader{ MessageType::JEWEL_TAKEN, sizeof(JewelTakenMessage) };
	std::vector<uint8_t> sendBuffer;
	sendBuffer.resize(sizeof(MessageHeader) + mh.size);
	memcpy(sendBuffer.data(), &mh, sizeof(MessageHeader));
	memcpy(sendBuffer.data() + sizeof(MessageHeader), &toSend, mh.size);
	broadcast(sendBuffer);
}