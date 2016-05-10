#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <map>
#include "Message.h"

class SpriteSheetLoader
{
private:
	std::map<std::string, sf::Texture*> contents;
	SpriteSheetLoader();
public:
	static SpriteSheetLoader* getInstance();
	sf::Texture* getSpriteSheet(std::string filepath);
};

class GameState;
class Entity
{
	uint8_t id;
	bool isPlayer;
	GameState* gamestate;

	bool isAlive;
	sf::Vector2i curr;
	sf::Vector2i next;
	sf::Vector2f pos;
	sf::Vector2f dir;
	int spriteDir;
	int spriteAction;
	int currCostume;

	bool moveUp;
	bool moveDown;
	bool moveLeft;
	bool moveRight;

	bool interact;
	bool shift;

	bool isMoving;
	float timeLeft;
	float frameTic;
	float frameCounter;

	sf::Texture* sheet;
	std::string* filepaths;
public:
	Entity(int row, int col, GameState* gamestate, bool isPlayer = false, uint8_t id = 0);
	void handleInput(const UpdateInputMessage& input);
	void update(float delta);
	void update(const UpdateDataMessage& e);
	void draw(sf::RenderWindow& window) const;
	UpdateDataMessage getData() const;
	void die();
};

#endif // ENTITY_H_INCLUDED
