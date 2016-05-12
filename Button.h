#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <SFML/Graphics.hpp>

class Button
{
private:
    bool hover;
    unsigned int fontHeight;
    sf::Font font;
    sf::Text label;
    sf::Vector2f center;
    sf::Vector2f half_diagonal;
    std::string content;
    sf::RectangleShape drop;
public:
    Button(sf::Vector2f center, sf::Vector2f half_diagonal, std::string content);
    bool checkCollision(float, float);
    void draw(sf::RenderWindow&) const;
};

#endif // BUTTON_H_INCLUDED
