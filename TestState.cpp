#include "State.h"

class TestState: public State {
    sf::CircleShape character;
    float x, y, xDisp, yDisp;
public:
    TestState() {
        character.setRadius(30);
        character.setOrigin(15, 15);
        character.setFillColor(sf::Color::Red);
        x = y = 20;
    }

    void handleInput () {
        xDisp = yDisp = 0;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            yDisp -= 500;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            yDisp += 500;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            xDisp -= 500;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            xDisp += 500;

    }

    void update (float dt) {

        x += xDisp*dt;
        y += yDisp*dt;

        character.setPosition(x, y);
    }

    void draw (sf::RenderWindow& window) const {
        window.draw(character);
    }
};
