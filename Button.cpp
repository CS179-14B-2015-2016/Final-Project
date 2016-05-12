#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Universal.h"
#include <stdlib.h>
#include <iostream>

Button::Button(sf::Vector2f center, sf::Vector2f half_diagonal, std::string content): center(center), half_diagonal(half_diagonal), content(content)
{
    font.loadFromFile("assets/fonts/black_jack.ttf");
    label = sf::Text(content, font);
    drop.setOrigin(half_diagonal);
    sf::Vector2f diagonal(half_diagonal.x*2, half_diagonal.y*2);
    drop.setSize(diagonal);
    drop.setPosition(center);
    drop.setFillColor(sf::Color::Black);

    label.setString(content);
    unsigned int font_size = (unsigned int)(half_diagonal.y*2*0.75);
    label.setCharacterSize(font_size);
    label.setOrigin(label.getLocalBounds().width/2, font_size/2.0);
    label.setPosition(center);
    hover = false;
}

bool Button::checkCollision(float u, float v)
{
    float targetX = Universal::clamp(u, center.x-half_diagonal.x, center.x+half_diagonal.x);
    float targetY = Universal::clamp(v, center.y-half_diagonal.y, center.y+half_diagonal.y);
    hover = (targetX==u && targetY==v);
    if(hover)
    {
        label.setColor(sf::Color::Black);
        drop.setFillColor(sf::Color::White);
    }
    else
    {
        label.setColor(sf::Color::White);
        drop.setFillColor(sf::Color(0,0,0,0));
    }
    return hover;
}

void Button::draw(sf::RenderWindow& window) const
{
    window.draw(drop);
    window.draw(label);
}
