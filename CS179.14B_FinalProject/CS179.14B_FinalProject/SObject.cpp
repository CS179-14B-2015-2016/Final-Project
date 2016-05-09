#include "SObject.h"
#include "Character.h"

bool SObject::can_be_destroyed() {
	return barrelDurability == 0;
}

void SObject::damage() {
	barrelDurability = std::max(0, barrelDurability - 1);
}

void HealBarrel::update(float dt) {}

void HealBarrel::collide(Entity* collided) {
	if (auto e = dynamic_cast<Character*>(collided)) {
		e->heal(1);
	}
}

void ExplodingBarrel::update(float dt) {}

void ExplodingBarrel::collide(Entity* collided) {
	if (auto e = dynamic_cast<Character*>(collided)) {
		//EXPLOSION ANIMATION?
		e->takeDamage(1);
	}
}

void PoisonousBarrel::update(float dt) {}

void PoisonousBarrel::collide(Entity* collided) {
	if (auto e = dynamic_cast<Character*>(collided)) {
		e->poison();
	}
}
