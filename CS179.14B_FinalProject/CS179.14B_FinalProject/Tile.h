#pragma once

#include "Entities.h"
#include "Game.h"

using namespace GAME;

class Tile : public Entity {
protected:

public:
	Tile(const sf::Vector2f &init_pos, std::string file_name) : Entity(file_name,sf::Vector2i(TILE_SIZE,TILE_SIZE)) {
		sprt.setPosition(init_pos + sf::Vector2f(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f));
		sprt.setOrigin(sf::Vector2f(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f));
	}
	virtual ~Tile() {}
	
	void virtual collide(Entity* collided) = 0;
};


class NormalTile : public Tile {
public:
	NormalTile(const sf::Vector2f &init_pos) : Tile(init_pos,GAME::NORMAL_FILE) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};

class LavaTile : public Tile {
protected:
	float timer;
	float damageTimer;
public:
	LavaTile(const sf::Vector2f &init_pos) : Tile(init_pos, GAME::LAVA_FILE), damageTimer(GAME::LAVA_TIMER), timer(0) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};

//added 3:12 25/03/2016
class DestructibleTile : public Tile {
protected:
	int tileDurability = 2;
	bool toDelete;
public:
	DestructibleTile(const sf::Vector2f &init_pos) : Tile(init_pos, GAME::NORMAL_FILE) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};

class TrampolineTile : public Tile {
public:
	TrampolineTile(const sf::Vector2f &init_pos) : Tile(init_pos, GAME::TRAMPOLINE_FILE) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};

class TarTile : public Tile {
public:
	TarTile(const sf::Vector2f &init_pos) : Tile(init_pos, GAME::TAR_FILE) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};


class TeleportationTile : public Tile {
public:
	TeleportationTile(const sf::Vector2f &init_pos) : Tile(init_pos, GAME::TELEPORTER_FILE) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};

class TrapTile : public Tile {
public:
	TrapTile(const sf::Vector2f &init_pos) : Tile(init_pos, "trap") {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};

class PermeableTile : public Tile {
public:
	PermeableTile(const int &size, const sf::Color &color, const sf::Vector2f &init_pos) : Tile(init_pos, "permeable") {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};
