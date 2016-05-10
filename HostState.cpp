#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include "StateManager.h"
#include "HostState.h"

HostState::HostState(StateManager* sm): State(sm), sm(sm),
back(sf::Vector2f(Universal::window_width/2.0-250, Universal::window_height/2.0+250), sf::Vector2f(100, 20), "Back"),
play(sf::Vector2f(Universal::window_width/2.0+250, Universal::window_height/2.0+250), sf::Vector2f(100, 20), "Play"),
name("Player 1"),
modeIndex(0),
mapIndex(0),
state(-1)
{
	modes = new std::string [6] { "Solo Game", "2 Players", "3 Players", "4 Players", "5 Players", "6 Players" };
	playerCount = new int [6] { 1, 2, 3, 4, 5, 6 };

	mapOption = new std::string[2] { "Generate", "Load" };

    font.loadFromFile("assets/fonts/black_jack.ttf");

    nameLabel = sf::Text("Name: ", font);
    nameInput = sf::Text(name, font);

    nameLabel.setPosition(Universal::window_width/2.0 - 200, Universal::window_height/2.0 - 150);
    nameInput.setPosition(Universal::window_width/2.0 + 10, Universal::window_height/2.0 - 150);

	modeLabel = sf::Text("Mode:", font);
	modeInput = sf::Text( modes[modeIndex], font);

	modeLabel.setPosition(Universal::window_width / 2.0 - 200, Universal::window_height / 2.0 - 100);
	modeInput.setPosition(Universal::window_width / 2.0 + 10, Universal::window_height / 2.0 - 100);

	mapLabel = sf::Text("Map Options:", font);
	mapInput = sf::Text( mapOption[mapIndex], font);

	mapLabel.setPosition(Universal::window_width / 2.0 - 200, Universal::window_height / 2.0 - 50);
	mapInput.setPosition(Universal::window_width / 2.0 + 10, Universal::window_height / 2.0 - 50);

	highlight.setFillColor(sf::Color::White);
	highlight.setSize(sf::Vector2f(200, 50));
}

void HostState::update(float dt) {
	if (state == -1) {
		highlight.setPosition(Universal::window_width / 2.0 - 200, Universal::window_height / 2.0 - 150);
		nameInput.setString(name);
		nameLabel.setColor(sf::Color::Black);
		modeLabel.setColor(sf::Color::White);
		mapLabel.setColor(sf::Color::White);
	}
	else if (state == 0) {
		highlight.setPosition(Universal::window_width / 2.0 - 200, Universal::window_height / 2.0 - 100);
		nameLabel.setColor(sf::Color::White);
		modeLabel.setColor(sf::Color::Black);
		mapLabel.setColor(sf::Color::White);
	}
	else if (state==1) {
		highlight.setPosition(Universal::window_width / 2.0 - 200, Universal::window_height / 2.0 - 50);
		nameLabel.setColor(sf::Color::White);
		modeLabel.setColor(sf::Color::White);
		mapLabel.setColor(sf::Color::Black);
	}

	modeInput.setString(modes[modeIndex]);
	mapInput.setString(mapOption[mapIndex]);
}

void HostState::handleInput(int u, int v, const std::string& accept, sf::Event e) {
	if(state==-1)
		Universal::appendTextInput(name, accept);

    bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	static bool prevLeftClick = false;
	bool leftMouseRelease = prevLeftClick && !leftClick;

	bool leftKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
	static bool prevLeftKey = false;
	bool leftKeyRelease = prevLeftKey && !leftKey;

	bool rightKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
	static bool prevRightKey = false;
	bool rightKeyRelease = prevRightKey && !rightKey;

	bool upKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
	static bool prevUpKey = false;
	bool upKeyRelease = prevUpKey && !upKey;

	bool downKey = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
	static bool prevDownKey = false;
	bool downKeyRelease = prevDownKey && !downKey;

	if (play.checkCollision(u, v) && leftMouseRelease) 
	{
		std::string toPass = "host " + mapOption[mapIndex] + " " + std::to_string(playerCount[modeIndex]) + " " + name;
		sm->push(4, toPass);
	}

	if (back.checkCollision(u, v) && leftMouseRelease)
	{
		sm->pop();
	}

	if (upKeyRelease && state != -1)
		state--;
	else if (downKeyRelease && state != 1)
		state++;

	if (leftKeyRelease) {
		if (state == 0)
		{
			if (modeIndex != 0)
				modeIndex--;
		}
		else if(state == 1)
		{
			if (mapIndex != 0)
				mapIndex--;
		}
	}


	if (rightKeyRelease) {
		if (state == 0)
		{
			if (modeIndex != 5)
				modeIndex++;
		}
		else if(state==1)
		{
			if (mapIndex != 1)
				mapIndex++;
		}
	}

	prevLeftClick = leftClick;
	prevLeftKey = leftKey;
	prevRightKey = rightKey;
	prevUpKey = upKey;
	prevDownKey = downKey;
}

void HostState::draw(sf::RenderWindow& rw) const {
    back.draw(rw);
	play.draw(rw);

	rw.draw(highlight);

    rw.draw(nameLabel);
    rw.draw(nameInput);
	rw.draw(modeLabel);
	rw.draw(modeInput);
	rw.draw(mapLabel);
	rw.draw(mapInput);
}
