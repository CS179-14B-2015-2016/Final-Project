#include "Tile.h"
#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Color.hpp"


Tile::Tile(float cx, float cy, int type, bool passability) {
	this->cx = cx;
	this->cy = cy;
	this->type = type;
	this->passability = passability;

	this->shape = sf::RectangleShape(sf::Vector2f(Tile::tileSize, Tile::tileSize));
	this->shape.setPosition(sf::Vector2f((cx - (Tile::tileSize / 2)), (cy - (Tile::tileSize / 2))));
	this->shape.setFillColor((type == 0)?(sf::Color::White):(sf::Color::Black));
	jewelSprite.loadFromFile("assets/sprites/jewel.png");
	toDraw = sf::Sprite(jewelSprite);
	toDraw.setPosition(sf::Vector2f((cx - (Tile::tileSize / 2)), (cy - (Tile::tileSize / 2))));
}

int Tile::getType() {
	return this->type;
}
void Tile::setColor(sf::Color color)
{
    this->shape.setFillColor(color);
}
// 0 is free tile, 1 is wall, 2 is object, 3 is hall, 4 is triggeredobject
void Tile::setType(int type) {
	this->type = type;
	if(type == 0) {
		this->passability = true;
		this->shape.setFillColor(sf::Color::Black);
	}
	else if (type == 1)
    {
        this->passability = false;
		this->shape.setFillColor(sf::Color::White);
    }
	else if (type == 2) {
		this->passability = false;
		this->shape.setFillColor(sf::Color::Yellow);
	}
	else if (type == 3) {
		this->passability = true;
		this->shape.setFillColor(sf::Color::Black);
	}
	else if (type == 4) {
		this->passability = false;
		this->shape.setFillColor(sf::Color::Red);
	}
}

bool Tile::isPassable() {
	return this->passability;
}

void Tile::draw(sf::RenderWindow* window) {
	if (type == 2)
		window->draw(toDraw);
	else
		window->draw(this->shape);
}
float Tile::getX(){
    return cx;
}
float Tile::getY(){
    return cx;
}
