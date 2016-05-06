#pragma once
#include "Entities.h"
#include "Game.h"
//#include "Character.h"

class Weapon : public Entity{
private:
	//Character *wielder;
	//int power;
protected:
	float anim_length;
	float curr_anim_length;
	//int left_attack_dt;
	//int right_attack_dt;
	int curr_attack;
	int left_attack;
	int right_attack;

public:
	int power;

	Weapon(string file_name, int weapon_width, int weapon_height) : 
		Entity(file_name, sf::Vector2i(weapon_width, weapon_height)) 
	{
		//wielder = wielder_input;
		sprt.setOrigin(sf::Vector2f(0, weapon_height / 2.0f));
		sprt.setRotation(270);
	}
	~Weapon(){}
	//virtual void fire(sf::Vector2f player_pos) = 0;
	//void update(float dt) override;
	//void handleMouse(int key, sf::RenderWindow &win);
	void render(sf::RenderTarget &g) override;
	void attack(int attack);
	int get_left_attack();
	int get_right_attack();
};

class Sword : public Weapon {
private:
	//Character *chara;
	//float anim_length;

public:

	Sword(): Weapon(GAME::WAR_SWORD_FILE, 24, 12)
	{
		anim_length = 0.2;
		curr_anim_length = anim_length;
		right_attack = 10;
		left_attack = -10;
	}

	void update(float dt);
	// void fire(sf::Vector2f player_pos) override;
};
