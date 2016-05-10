#include "Entity.h"
#include "Universal.h"
#include <cassert>
#include "GameState.h"

SpriteSheetLoader::SpriteSheetLoader() {
}

SpriteSheetLoader* SpriteSheetLoader::getInstance() {
	static SpriteSheetLoader ssl;
	return &ssl;
}

sf::Texture* SpriteSheetLoader::getSpriteSheet(std::string filepath) {
	if (contents.find(filepath) == contents.end())
	{
		contents[filepath] = new sf::Texture();
		contents[filepath]->loadFromFile(filepath);
	}
	return contents[filepath];
}

Entity::Entity(int row, int col, GameState* gamestate, bool isPlayer, uint8_t id) :
	id(id),
	isPlayer(isPlayer),
	gamestate(gamestate),
	isAlive(true),
	curr(row, col),
	next(row, col),
	pos(row * 32, col * 32),
	dir(0, 0),
	score(0),
	spriteDir(0),
	spriteAction(0),
	currCostume(0),
	moveUp(false),
	moveDown(false),
	moveLeft(false),
	moveRight(false),
	interact(false),
	shift(false),
	isMoving(false),
	timeLeft(0),
	frameTic( 1 / 4.0f ),
	frameCounter(0)
{
	filepaths = new std::string[6]{
		"assets/sprites/lady1.png",
		"assets/sprites/lady2.png",
		"assets/sprites/lady3.png",
		"assets/sprites/man1.png",
		"assets/sprites/man2.png",
		"assets/sprites/man3.png" };
	currCostume = rand() % 6;
	sheet = SpriteSheetLoader::getInstance()->getSpriteSheet(filepaths[ currCostume ]);
}

void Entity::handleInput(const UpdateInputMessage& input)
{
	if (!isAlive)
		return;

	assert(this->id == input.ID);
	this->moveUp = input.moveUp;
	this->moveDown = input.moveDown;
	this->moveLeft = input.moveLeft;
	this->moveRight = input.moveRight;
	this->interact = input.interact;
	this->shift = input.shift;
}

void Entity::update(float delta)
{
	if (!isAlive)
		return;

	if (isMoving)
	{
		pos += delta * (float)(32.0 / 0.3) * dir;
		timeLeft -= delta;
		frameCounter += delta;
		if (frameCounter >= frameTic)
		{
			frameCounter -= frameTic;
			if (spriteAction == 1)
				spriteAction = 2;
			else if (spriteAction == 2)
				spriteAction = 1;
		}
		if (timeLeft <= 0)
		{
			timeLeft = 0;
			frameCounter = 0;
			pos = sf::Vector2f(next.x*32, next.y*32);
			dir = sf::Vector2f(0, 0);
			gamestate->freeMap(curr.x, curr.y);
			curr = next;
			spriteAction = 0;
			isMoving = false;
		}
	}
	else
	{
		auto nextX = curr.x;
		auto nextY = curr.y;

		if (spriteDir == 3)
			nextX--;
		else if (spriteDir == 0)
			nextX++;
		else if (spriteDir == 1)
			nextY--;
		else if (spriteDir == 2)
			nextY++;

		if (!isPlayer)
		{
			auto randomValue = rand() % 2;

			if (randomValue == 0 && gamestate->isMapFree(nextX, nextY))
			{
				isMoving = true;
			}
			else
			{
				moveUp = moveDown = moveLeft = moveRight = false;
				randomValue = rand() % 5;

				if (randomValue==0)			moveUp = true;
				else if (randomValue==1)	moveDown = true;
				else if (randomValue==2)	moveLeft = true;
				else if (randomValue==3)	moveRight = true;
				else						timeLeft = 0.6;
			}
		}
		
		if (isPlayer && interact) 
		{
			auto objectCode = gamestate->getEntityCode(nextX, nextY);
			if (objectCode == 2)
			{
				score++;
				gamestate->takeJewel(nextX, nextY);
			}
			else if (objectCode == 8)
			{
				auto object = gamestate->getEntityAt(nextX, nextY);
				if (object != nullptr && object->spriteDir==this->spriteDir)
				{
					if (object->isPlayer)
					{
						this->score += object->score;
						object->score = 0;
					}
					else
					{
						this->score = 0;
						this->die();
					}
					object->die();
				}
			}

			interact = false;
		}

		if (isPlayer && shift)
		{
			auto objectCode = gamestate->getEntityCode(nextX, nextY);
			if (objectCode == 8)
			{
				auto object = gamestate->getEntityAt(nextX, nextY);
				if (object != nullptr && object->isAlive)
				{
					this->currCostume = object->currCostume;
				}
			}

			interact = false;
		}

		if (moveUp && !moveDown)
		{
			spriteDir = 3;
			if (gamestate->isMapFree(curr.x - 1, curr.y))
			{
				timeLeft = 0.3;
				gamestate->reserveMap(curr.x - 1, curr.y);
				isMoving = true;
				dir = sf::Vector2f(-1, 0);
				next = sf::Vector2i(curr.x - 1, curr.y);
				spriteAction = 1;
			}
		}
		else if (moveDown && !moveUp)
		{
			spriteDir = 0;
			if (gamestate->isMapFree(curr.x + 1, curr.y))
			{
				timeLeft = 0.3;
				gamestate->reserveMap(curr.x + 1, curr.y);
				isMoving = true;
				dir = sf::Vector2f(1, 0);
				next = sf::Vector2i(curr.x + 1, curr.y);
				spriteAction = 1;
			}
		}
		else if (moveLeft && !moveRight)
		{
			spriteDir = 1;
			if (gamestate->isMapFree(curr.x, curr.y - 1))
			{
				timeLeft = 0.3;
				gamestate->reserveMap(curr.x, curr.y - 1);
				isMoving = true;
				dir = sf::Vector2f(0, -1);
				next = sf::Vector2i(curr.x, curr.y - 1);
				spriteAction = 1;
			}
		}
		else if (moveRight && !moveLeft)
		{
			spriteDir = 2;
			if (gamestate->isMapFree(curr.x, curr.y + 1)) 
			{
				timeLeft = 0.3;
				gamestate->reserveMap(curr.x, curr.y + 1);
				isMoving = true;
				dir = sf::Vector2f(0, 1);
				next = sf::Vector2i(curr.x, curr.y + 1);
				spriteAction = 1;
			}
		}
	}
}

void Entity::update(const UpdateDataMessage& data)
{
	if (!isAlive)
		return;

	assert(this->id == data.ID);

	this->isAlive = data.isAlive;
	this->pos = sf::Vector2f( data.posX, data.posY );
	this->spriteDir = data.spriteDir;
	this->spriteAction = data.spriteAction;
	this->currCostume = data.currCostume;

	sheet = SpriteSheetLoader::getInstance()->getSpriteSheet(filepaths[currCostume]);
}

UpdateDataMessage Entity::getData() const {
	if (id == 2)
		std::cout << "Telling the future I am wearing the costume " << currCostume << std::endl;
	return UpdateDataMessage{ id, isAlive, pos.x, pos.y, spriteDir, spriteAction, currCostume };
}

void Entity::draw(sf::RenderWindow& window) const {
	if (isAlive)
	{
		sf::Sprite toDraw(*sheet, sf::IntRect(spriteAction * 32, spriteDir * 32, 32, 32));
		toDraw.setPosition(pos.y, pos.x);
		window.draw(toDraw);
	}
}

void Entity::die() {
	isAlive = false;
	gamestate->freeMap(curr.x, curr.y);
}

sf::Vector2i Entity::getPosition() const
{
	return curr;
}