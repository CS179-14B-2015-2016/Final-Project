#pragma once

#include "Entities.h"
#include "Weapon.h"
#include "GameMessage.h"
#include "Game.h"

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
	playerChar type;
	int str, agi, intel, pdef, mdef, currhealth, maxhealth, seq;
	float poison_timer;
	float velXMultiplier;
	Face currface;
	sf::Vector2f vel;
	sf::Vector2f acc;
	
	Weapon* weap;
	
	bool can_jump;
	bool isColTile;
	
public:
	Character(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, string file_name,ID id, Weapon *weap_in, playerChar type) : 
		Entity(file_name, sf::Vector2i(CHARACTERS::SPRITE_WIDTH,CHARACTERS::SPRITE_HEIGHT)), str(str), agi(agi), intel(intel), pdef(pdef),
		mdef(mdef), currhealth(maxhealth), maxhealth(maxhealth), id(id), can_jump(true), poison_timer(CHARACTERS::POISON_MAX_TIME), isColTile(false),velXMultiplier(1), weap(weap_in),type(type) {
		sprt.setPosition(startPos);
		sprt.setOrigin(sf::Vector2f(CHARACTERS::SPRITE_WIDTH / 2.0f, CHARACTERS::SPRITE_HEIGHT / 2.0f));
		currface = Face::NONE;
		seq;
	}
	virtual ~Character() {}

	virtual void Attack() = 0;
	virtual void SAttack() = 0;
	bool isKeyDown(const int &key);
	
	void resetGravity();
	void hit_head();
	void setCollision(bool col);
	void handleInput();
	int handleMouse(int key, sf::RenderWindow &win);
	
	void takeDamage(int damage);
	void heal(int heal);
	void slow();
	void boosted();
	void poison();
	
	void update(float dt) override;
	void update(sf::Vector2f pos, sf::Vector2f vel,Face face);
	void render(sf::RenderTarget &g) override;
	
	ID getId() const;
	playerChar getType() const;
	Face getFace() const;
	int getHealth() const;
	int get_strength() const;
	sf::Vector2f getVel() const;
	Weapon* get_weapon() const;
	// sf::Rect<float> getXColBox() const;
	// sf::Rect<float> getYColBox() const;
};


class War : public Character {

public:
	War(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) : 
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::WAR_FILE, id, new Sword(),playerChar::WAR){}
	void Attack() override;
	void SAttack() override;
};



class Famine : public Character {
public:
	Famine(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) :
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::FAM_FILE, id, new Sword(), playerChar::FAMINE) {}
	void Attack() override;
	void SAttack() override;
};



class Pestilence : public Character {

public:
	Pestilence(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) :
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::PESTE_FILE, id, new Sword(), playerChar::PESTILENCE) {}
	void Attack() override;
	void SAttack() override;
};



class Death : public Character {

public:
	Death(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) :
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::DEATH_FILE, id, new Sword(), playerChar::DEATH) {}
	void Attack() override;
	void SAttack() override;
};


class Matthew : public Character {

public:
	Matthew(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) :
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::MATTHEW_FILE, id, new Sword(), playerChar::MATTHEW) {}
	void Attack() override;
	void SAttack() override;
};


class Mark : public Character {

public:
	Mark(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) :
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::MARK_FILE, id, new Sword(), playerChar::MARK) {}
	void Attack() override;
	void SAttack() override;
};


class Luke : public Character {

public:
	Luke(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) :
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::LUKE_FILE, id, new Sword(), playerChar::LUKE) {}
	void Attack() override;
	void SAttack() override;
};


class John : public Character {

public:
	John(const int &str, const int &agi, const int &intel, const int &pdef, const int &mdef,
		int maxhealth, const sf::Vector2f &startPos, ID id) :
		Character(str, agi, intel, pdef, mdef, maxhealth, startPos, GAME::JOHN_FILE, id, new Sword(), playerChar::JOHN) {}
	void Attack() override;
	void SAttack() override;
};
