#pragma once

#include "TextureLoader.h"

#include <SFML/Graphics.hpp>

class Entity {
protected:
	sf::Sprite sprt;
public:
	Entity(const std::string &file_name, const sf::Vector2i &texPosSize,const sf::Vector2i &initPosTex = sf::Vector2i(0,0)) {
		sprt.setTexture(*tl.getTexture(file_name));
		sprt.setTextureRect(sf::Rect<int>(initPosTex.x, initPosTex.y, texPosSize.x /*width*/, texPosSize.y /*height*/));
	}
	virtual ~Entity() {}
	
	virtual void move(const sf::Vector2f &amount);
	
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderTarget &g);
	
	virtual void setPos(const sf::Vector2f &newPos); 
	virtual void setTextRect(const sf::Rect<int> newRekt);
	virtual	void setOrigin(const sf::Vector2f &newOrig);
	
	virtual sf::Vector2f getPosition() const;
	
	virtual sf::Rect<float> bounds() const;
};
