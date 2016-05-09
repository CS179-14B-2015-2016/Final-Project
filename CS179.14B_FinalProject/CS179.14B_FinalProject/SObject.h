#pragma once

#include "Entities.h"
#include "Game.h"

using namespace GAME;

class SObject : public Entity {
protected:
	int barrelDurability;
	
public:
	SObject(const sf::Vector2f &init_pos, std::string file_name, int barrelDurability) : Entity(file_name, sf::Vector2i(SOBJECT_SIZE, SOBJECT_SIZE)), barrelDurability(barrelDurability) {
		sprt.setPosition(init_pos + sf::Vector2f(SOBJECT_SIZE / 2.0f, SOBJECT_SIZE / 2.0f));
		sprt.setOrigin(sf::Vector2f(SOBJECT_SIZE / 2.0f, SOBJECT_SIZE / 2.0f));
	}
	virtual ~SObject() {}
	
	void virtual collide(Entity* collided) = 0;
	bool can_be_destroyed();
	void damage();
};

class HealBarrel : public SObject {
public:
	HealBarrel(const sf::Vector2f &init_pos) : SObject(init_pos, GAME::HEALB_FILE, 1) {}
	void update(float dt) override;
	void collide(Entity* collided) override;

};

class ExplodingBarrel : public SObject {
public:
	ExplodingBarrel(const sf::Vector2f &init_pos) : SObject(init_pos, GAME::EXPLODEB_FILE, 10) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};

class PoisonousBarrel : public SObject {
public:
	PoisonousBarrel(const sf::Vector2f &init_pos) : SObject(init_pos, GAME::POISONB_FILE, 10) {}
	void update(float dt) override;
	void collide(Entity* collided) override;
};
