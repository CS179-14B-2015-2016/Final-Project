#pragma once

#include "Entities.h"
#include "Weapon.h"
#include "Game.h"
#include "../GameMessage.h"

#include <Windows.h>

#define WIN32_LEAN_AND_MEAN

const float GRAV = 10.0f; // 9.8f / FPS;

namespace Keys {
	const auto JUMP = 0x11;     // W
	const auto DOWN = 0x1F;		// S
	const auto LEFT = 0x1E;  	// A
	const auto RIGHT = 0x20; 	// D
}

namespace CHARACTERS {
	const auto BASE_SPEED = 32.0f;
	const auto SPRITE_WIDTH = 32;
	const auto SPRITE_HEIGHT = 48;
	const auto JUMP_RATE = -400.0f;
	const auto HEALTHBAR_WIDTH = 40;
	const auto HEALTHBAR_HEIGHT = 5;
	const auto BOOST_JUMP = 2*JUMP_RATE;
	const auto POISON_DAMAGE = 1;
	const auto POISON_MAX_TIME = 5;
	const auto POISON_FREQ = 1.5;
}

class Character : public Entity {
private:
	ID id;
	Weapon* weap;
	playerChar type;
	
	int str, agi, intel, pdef, mdef, currhealth, maxhealth, seq;
	
	Face currface;
	sf::Vector2f vel;
	sf::Vector2f acc;
	
	float poison_timer;
	float velXMultiplier;
	bool can_jump;
	
	int death_count;
	
public:
	Character(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef, int maxhealth, string file_name,ID id, Weapon *weap_in, playerChar type) : 
		Entity(file_name, sf::Vector2i(CHARACTERS::SPRITE_WIDTH,CHARACTERS::SPRITE_HEIGHT)), str(str), agi(agi), intel(intel), pdef(pdef), mdef(mdef), currhealth(maxhealth), maxhealth(maxhealth), seq(0), id(id), can_jump(true), poison_timer(CHARACTERS::POISON_MAX_TIME), velXMultiplier(1), weap(weap_in), type(type), death_count(0) {
		sprt.setOrigin(sf::Vector2f(CHARACTERS::SPRITE_WIDTH / 2.0f, CHARACTERS::SPRITE_HEIGHT / 2.0f));
		currface = Face::NONE;
	}
	virtual ~Character() {}

	virtual void Attack() = 0;
	virtual void SAttack() = 0;
	bool isKeyDown(const int &key);
	
	void resetGravity();
	void hit_head();
	
	void handleInput();
	int handleMouse(int key, sf::RenderWindow &win);
	
	void takeDamage(int damage);
	void heal(int heal);
	void slow();
	void boosted();
	void poison();
	
	void update(float dt) override;
	void update(sf::Vector2f pos, int hp);
	void render(sf::RenderTarget &g) override;
	
	ID getId() const;
	playerChar getType() const;
	Face getFace() const;
	int getHealth() const;
	int get_strength() const;
	sf::Vector2f getVel() const;
	Weapon* get_weapon() const;
};

class War : public Character {
public:
	War(ID id) : Character(10, 7, 2, 7, 3, 20, GAME::WAR_FILE, id, new Sword(), playerChar::WAR) {}
	void Attack() override;
	void SAttack() override;
};

class Famine : public Character {
public:
	Famine(ID id) : Character(2, 7, 10, 3, 7, 20, GAME::FAM_FILE, id, new Sword(), playerChar::FAMINE) {}
	void Attack() override;
	void SAttack() override;
};

class Pestilence : public Character {
public:
	Pestilence(ID id) : Character(7, 10, 2, 5, 5, 20, GAME::PESTE_FILE, id, new Sword(), playerChar::PESTILENCE) {}
	void Attack() override;
	void SAttack() override;
};

class Death : public Character {
public:
	Death(ID id) : Character(10, 2, 7, 3, 7, 20, GAME::DEATH_FILE, id, new Sword(), playerChar::DEATH) {}
	void Attack() override;
	void SAttack() override;
};

class Matthew : public Character {
public:
	Matthew(ID id) : Character(2, 7, 10, 3, 7, 20, GAME::MATTHEW_FILE, id, new Sword(), playerChar::MATTHEW) {}
	void Attack() override;
	void SAttack() override;
};

class Mark : public Character {
public:
	Mark(ID id) : Character(10, 7, 2, 7, 3, 20, GAME::MARK_FILE, id, new Sword(), playerChar::MARK) {}
	void Attack() override;
	void SAttack() override;
};

class Luke : public Character {
public:
	Luke(ID id) : Character(10, 7, 2, 7, 3, 20, GAME::LUKE_FILE, id, new Sword(), playerChar::LUKE) {}
	void Attack() override;
	void SAttack() override;
};

class John : public Character {
public:
	John(ID id) : Character(7, 10, 2, 7, 3, 20, GAME::JOHN_FILE, id, new Sword(), playerChar::JOHN) {}
	void Attack() override;
	void SAttack() override;
};
