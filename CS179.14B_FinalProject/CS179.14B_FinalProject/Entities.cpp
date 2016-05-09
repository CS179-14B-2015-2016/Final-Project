#include "Entities.h"

void Entity::render(sf::RenderTarget &g) {
	g.draw(sprt);
}

void Entity::move(const sf::Vector2f &amount) {
	sprt.move(amount);
}

void Entity::setPosition(const sf::Vector2f &newPos) {
	sprt.setPosition(newPos);
}

void Entity::setTextRect(const sf::Rect<int> newRekt) {
	sprt.setTextureRect(newRekt);
}

void Entity::setOrigin(const sf::Vector2f &newOrig) {
	sprt.setOrigin(newOrig);
}

sf::Vector2f Entity::getPosition() const {
	return sprt.getPosition();
}

sf::Rect<float> Entity::bounds() const {
	return sprt.getGlobalBounds();
}