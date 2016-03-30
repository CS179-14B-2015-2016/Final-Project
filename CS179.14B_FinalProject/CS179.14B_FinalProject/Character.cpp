#include "Character.h"

bool Character::isKeyDown(const int &key) {
	auto state = GetAsyncKeyState(MapVirtualKey(key, MAPVK_VSC_TO_VK_EX));
	return state >> 15 != 0;
}
void Character::handleMouse(int key) {
	if (key == 1) {
		//Attack();
		cout << "PEWPEW" << endl;
		key = 0;
	}
	if (key == 2) {
		//SAttack();
		cout << "KABLAM" << endl;
		key = 0;
	}
}

void Character::handleInput(){
	vel.x = 0.0;
	Face temp = Face::NONE;
	if (isKeyDown(Keys::UP)) {/*jump*/
		temp = Face::FRONT;
		vel.y = CHARACTERS::JUMP_RATE;
	}
	else if (isKeyDown(Keys::DOWN)) {/*drop down?*/ }
	if (isKeyDown(Keys::LEFT)) {
		temp = Face::LEFT;
		vel.x = -CHARACTERS::BASE_SPEED * agi;
	}
	else if (isKeyDown(Keys::RIGHT)) {
		temp = Face::RIGHT;
		vel.x = CHARACTERS::BASE_SPEED * agi;
	}

	if (temp == currface) {
		seq++;
		if (seq > 3) {
			seq = 0;
		}
	}
	else {
		currface = temp;
		seq = 0;
	}
	int frameY = 0;
	switch (currface) {
	case UP:
		frameY = 3 * CHARACTERS::SPRITE_HEIGHT;
		break;
	case FRONT:
		frameY = 0;
		break;
	case NONE:
		frameY = 0;
		seq = 0;
		break;
	case LEFT:
		frameY = CHARACTERS::SPRITE_HEIGHT;
		break;
	case RIGHT:
		frameY = 2 * CHARACTERS::SPRITE_HEIGHT;
		break;
	}
	sprt.setTextureRect(sf::Rect<int>(seq*CHARACTERS::SPRITE_WIDTH, frameY, CHARACTERS::SPRITE_WIDTH, CHARACTERS::SPRITE_HEIGHT));
}


void Character::update(float dt) {
	if (!isColliding[1]) {
		vel.y += GRAV;
	}
	sprt.setPosition(sprt.getPosition() + vel*dt);
	view.setCenter(sprt.getPosition());
}

void Character::resetGravity() {
	vel.y = 0;
}

void Character::boosted() {
	vel.y = CHARACTERS::BOOST_JUMP;
}

void Character::slow() {
	vel.x *= .5;
}

void Character::takeDamage(int damage){
	currhealth -= damage;
}
void Character::heal(int heal){
	currhealth += heal;
}

sf::Rect<float> Character::bounds() const {
	sf::Rect<float> toRet(sprt.getGlobalBounds());
	toRet.width -= 9;
	toRet.height -= 4;
	return toRet;
}

sf::Vector2f Character::getVel() const {
	return vel;
}

sf::Rect<float> Character::getXColBox() const {
	sf::Rect<float> toRet(sprt.getGlobalBounds());
	toRet.height -= 10;
	toRet.top += 5;
	return toRet;
}

sf::Rect<float> Character::getYColBox() const {
	sf::Rect<float> toRet(sprt.getGlobalBounds());
	toRet.width -= 10;
	toRet.left += 5;
	return toRet;
}

void Character::render(sf::RenderTarget &g) {
	Entity::render(g);
	g.setView(view);
}

void War::Attack() {}
void War::SAttack() {}

