#include "Weapon.h"
#include "Game.h"

void Weapon::render(sf::RenderTarget &g) {
	Entity::render(g);
}

void Weapon::attack(int attack) {
	_is_attacking = true;
	curr_anim_length = 0;
	curr_attack = attack;
}

void Weapon::hit() {
	_is_attacking = false;
}

int Weapon::get_left_attack() {
	return left_attack;
}

int Weapon::get_right_attack() {
	return right_attack;
}

bool Weapon::is_attacking() {
	return _is_attacking;
}

void Sword::update(float dt) {
	curr_anim_length += dt;
	if (curr_anim_length < anim_length) {
		sprt.rotate(curr_attack);
	} else {
		_is_attacking = false;
		sprt.setRotation(270);
	}
}
