#include "Tile.h"
#include "Character.h"

//added 9:12 pm 25/03/2016
void NormalTile::update(float dt) {}
void NormalTile::collide(Entity* collided) {}

void LavaTile::update(float dt) {
	if( timer < damageTimer){
		timer += dt;
	}
}
void LavaTile::collide(Entity* collided){
	if (auto e = dynamic_cast<Character*>(collided)) {
		// cout << "Do Damage\n";
		if (timer >= damageTimer){
			timer = 0;
			e->takeDamage(3);
		}
	}
}

void DestructibleTile::update(float dt) {}

void DestructibleTile::collide(Entity* collided){
	tileDurability--;
	if(tileDurability <= 0){
		toDelete = true;
	}
}

void TrampolineTile::update(float dt) {}

void TrampolineTile::collide(Entity* collided){
	if (auto e = dynamic_cast<Character*>(collided)) {
		// cout << "Jump Higher\n";
		e->boosted();
	}
}

void TarTile::update(float dt) {}

void TarTile::collide(Entity* collided){
	if (auto e = dynamic_cast<Character*>(collided)) {
		// cout << "slow " << endl;
		e->slow();
	}
}

/*
void TeleportationTile::update(float dt) {}
void TeleportationTile::collide(Entity* collided){}

void TrapTile::update(float dt) {}
void TrapTile::collide(Entity* collided){}

void PermeableTile::update(float dt) {}
void PermeableTile::collide(Entity* collided){}
*/
