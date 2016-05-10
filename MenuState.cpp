#include "Universal.h"
#include "MenuState.h"
#include "StateManager.h"
#include <iostream>
#include <cstdlib>


MenuState::MenuState(StateManager *sm):
State(sm),
sm(sm),
join(Button(sf::Vector2f(Universal::window_width/2.0, Universal::window_height/2.0 + 50), sf::Vector2f(100, 20), "Join Game")),
host(Button(sf::Vector2f(Universal::window_width/2.0, Universal::window_height/2.0 + 100), sf::Vector2f(100, 20), "Host Game")),
options(Button(sf::Vector2f(Universal::window_width/2.0, Universal::window_height/2.0 + 150), sf::Vector2f(100, 20), "Options")),
exit(Button(sf::Vector2f(Universal::window_width/2.0, Universal::window_height/2.0 + 200), sf::Vector2f(100, 20), "Exit"))
{
    std::cout<< "MENU STATE" <<std::endl;
    cover = 255;
}

void MenuState::update(float dt)
{
    if(cover==0) return;
    int temp = cover - (255*dt);
    cover = std::max(temp, 0);
}

void MenuState::handleInput(int u, int v, const std::string& typed, sf::Event e)
{
    bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    if(leftClick && cover > 0) {
        cover = 0;
        return;
    }
    if(join.checkCollision(u, v) && leftClick){
        sm->push(2, "");
    }
    if(host.checkCollision(u, v) && leftClick) {
        sm->push(3, "");
    }
    if(options.checkCollision(u, v) && leftClick) {
		sm->push(6, "");
    }
    if(exit.checkCollision(u, v) && leftClick) {
        std::exit(0);
    }
}

void MenuState::draw(sf::RenderWindow &window) const
{
    join.draw(window);
    host.draw(window);
    options.draw(window);
    exit.draw(window);

    if(cover==0) return;
    sf::Vector2f curtainSize(800, 600);
    sf::RectangleShape curtain(curtainSize);
    curtain.setPosition(0, 0);
    curtain.setFillColor(sf::Color(0, 0, 0, cover));
    window.draw(curtain);
}
