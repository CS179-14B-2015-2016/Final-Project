#pragma once

#include "Character.h"
#include "Weapon.h"
#include "Tile.h"
#include "TextureLoader.h"
#include "SObject.h"
#include "Game.h"

#include "SFML/Network.hpp"

#include <vector>

class EntityManager {
private:
	sf::View view;
	
public:
	std::vector<Tile*> map;
	Character* main_player;
	std::vector<Character*> other_players;
	std::vector<SObject*> sobjects;
	sf::Sprite bg;
	sf::UdpSocket* socket;
	sf::IpAddress address;
	unsigned int port;
	// socket.send(buffer, sizeof(buffer), server_address, port);
	
	EntityManager(sf::UdpSocket* sock,const char* ip,const unsigned int &port) : socket(sock), address(ip), port(port) {
		view.setSize(sf::Vector2f(GAME::WINDOW_WIDTH, GAME::WINDOW_HEIGHT));
		// bg.setTexture(*tl.getTexture("Art/Maps/bg_temp.png"));
	}
	
	virtual ~EntityManager() {}
	
	void addMapTile(Tile* t);
	void addSObject(SObject* so);
	
	void setMain(Character* p, const sf::Vector2f& start_pos);
	
	void handleInput();
	void handleMouse(int key, sf::RenderWindow &g);
	
	void update(float dt);
	void render(sf::RenderTarget &g);
	
	void logic(float dt);
	void resolveCollisions(float dt);
	bool has_collided(Weapon* w, Character* p);
	bool has_collided(Weapon* w, SObject* t);
	bool has_collided(Tile* t, Character* p);
	bool has_collided(SObject* t, Character* p);
};
