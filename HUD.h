#ifndef HUD_H_INCLUDED
#define HUD_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <string>

class HUD
{
private:
	sf::Font font;
	sf::Text timer;
	sf::Text jewels;
	sf::Texture jewelTexture;
	sf::Sprite jewelSprite;
	float timeRemaining;
	int jewelCount;
public:
	HUD();
	bool update(float dt);
	void draw(sf::RenderWindow &rw) const;
	void setJewelCount(int i);
};

#endif //HUD_H_INCLUDED
