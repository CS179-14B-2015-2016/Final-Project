#pragma once
#include "Character.h"
#include "Weapon.h"
#include "Tile.h"
#include "TextureLoader.h"
#include "SObject.h"
#include "SFML/Network.hpp"
#include <vector>

class EntityManager {
public:
	std::vector<Tile*> map;
	Character* main_player;
	std::vector<Character*> other_players;
	std::vector<SObject*> sobjects;
	sf::Sprite bg;
	sf::UdpSocket* socket;
	sf::IpAddress address;
	unsigned int port;
	//    socket.send(buffer, sizeof(buffer), server_address, port);
	EntityManager(sf::UdpSocket* sock,const char* ip,const unsigned int &port) : socket(sock), address(ip), port(port) {
		//bg.setTexture(*tl.getTexture("Art/Maps/bg_temp.png"));
	}
	virtual ~EntityManager() {}
	void addPlayer(Character* p);
	void setMain(Character* p);
	void addMapTile(Tile* t);
	void addSObject(SObject* so);
	void handleInput();
	void handleMouse(int key, sf::RenderWindow &g);
	void update(float dt);
	void logic(float dt);
	void render(sf::RenderTarget &g);
	void resolveCollisions(float dt);
	bool has_collided(Weapon* w, Character* p);
	bool has_collided(Weapon* w, SObject* t);
	bool has_collided(Tile* t, Character* p);
	bool has_collided(SObject* t, Character* p);
};
