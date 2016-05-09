#pragma once

#include "Entities.h"
#include "Game.h"

class Weapon : public Entity{
protected:
	float anim_length;
	float curr_anim_length;
	
	bool _is_attacking;
	
	int curr_attack;
	int left_attack;
	int right_attack;

public:
	int power;

	Weapon(string file_name, int weapon_width, int weapon_height) : Entity(file_name, sf::Vector2i(weapon_width, weapon_height)), power(10), _is_attacking(false) {
		sprt.setOrigin(sf::Vector2f(0, weapon_height / 2.0f));
		sprt.setRotation(270);
	}
	~Weapon() {}
	
	void render(sf::RenderTarget &g) override;
	void attack(int attack);
	void hit();
	
	bool is_attacking();
	
	int get_left_attack();
	int get_right_attack();
};

class Sword : public Weapon {
public:
	Sword(): Weapon(GAME::WAR_SWORD_FILE, 24, 12) {
		anim_length = 0.2;
		curr_anim_length = anim_length;
		right_attack = 10;
		left_attack = -10;
	}

	void update(float dt) override;
};
