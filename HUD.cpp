#include "HUD.h"
#include "Universal.h"
#include <string>

HUD::HUD() :
	timeRemaining( Universal::GAME_DURATION ),
	jewelCount( 0 )
{
	font = sf::Font();
	font.loadFromFile("assets/fonts/black_jack.ttf");
	timer.setFont(font);
	timer.setCharacterSize(30);
	jewels.setFont(font);
	jewels.setCharacterSize(30);

	jewelTexture.loadFromFile("assets/sprites/jewel.png");
	jewelSprite = sf::Sprite(jewelTexture);
	jewelSprite.scale(0.75f, 0.75f);
	jewelSprite.setPosition(10, 10);

	jewels.setPosition(34, 10);
	jewels.setColor(sf::Color::Green);
	timer.setPosition(10 , 44);
	timer.setColor(sf::Color::Green);
}

bool HUD::update(float delta) 
{
	timeRemaining -= delta;
	int onlyInteger = (int)(timeRemaining);

	int minTen = (onlyInteger / 60) / 10;
	int minOne = (onlyInteger / 60);
	int secTen = (onlyInteger % 60) / 10;
	int secOne = (onlyInteger % 60) % 10;
	timer.setString(std::to_string(minTen) + std::to_string(minOne) + ":" + std::to_string(secTen) + std::to_string(secOne));
	jewels.setString(" x " + std::to_string( jewelCount ));

	return timeRemaining <= 0;
}

void HUD::setJewelCount(int i) {
	jewelCount = i;
}

void HUD::draw(sf::RenderWindow& rw) const {
	rw.draw(timer);
	rw.draw(jewels);
	rw.draw(jewelSprite);
}