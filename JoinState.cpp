#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "StateManager.h"
#include "JoinState.h"

JoinState::JoinState(StateManager* sm): State(sm), sm(sm),
back(sf::Vector2f(Universal::window_width/2.0-250, Universal::window_height/2.0+250), sf::Vector2f(100, 20), "Back"),
play(sf::Vector2f(Universal::window_width/2.0+250, Universal::window_height/2.0+250), sf::Vector2f(100, 20), "Play"),
ip("localhost"), name("Player 2"), state(0)
{
    font.loadFromFile("assets/fonts/black_jack.ttf");
    ipLabel = sf::Text("IP Address: ", font);
    ipInput = sf::Text(ip, font);
    nameLabel = sf::Text("Name: ", font);
    nameInput = sf::Text(name, font);

    ipLabel.setPosition(Universal::window_width/2.0 - 150, Universal::window_height/2.0 - 50);
    ipInput.setPosition(Universal::window_width/2.0 + 50, Universal::window_height/2.0 - 50);
    nameLabel.setPosition(Universal::window_width/2.0 - 150, Universal::window_height/2.0 + 50);
    nameInput.setPosition(Universal::window_width/2.0 + 50, Universal::window_height/2.0 + 50);

    highlight.setFillColor(sf::Color::White);
    highlight.setSize(sf::Vector2f(150, 50));
}

void JoinState::update(float dt) {
    ipInput.setString(ip);
    nameInput.setString(name);
    if(state==0)
    {
        ipLabel.setColor(sf::Color::Black);
        nameLabel.setColor(sf::Color::White);
        highlight.setPosition(Universal::window_width/2.0 - 155, Universal::window_height/2.0 - 55);
    }
    else if(state==1)
    {
        nameLabel.setColor(sf::Color::Black);
        ipLabel.setColor(sf::Color::White);
        highlight.setPosition(Universal::window_width/2.0 - 155, Universal::window_height/2.0 + 45);
    }
    else
    {
        ipLabel.setColor(sf::Color::White);
        nameLabel.setColor(sf::Color::White);
    }
}

void JoinState::handleInput(int u, int v, const std::string& accept, sf::Event e) {
    bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    bool upCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    static bool upPreviouslyPressed = false;

    bool dnCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    static bool dnPreviouslyPressed = false;

    bool upReleased = !upCurrentlyPressed && upPreviouslyPressed;
    bool dnReleased = !dnCurrentlyPressed && dnPreviouslyPressed;

    if(back.checkCollision(u, v) && leftClick)
        popSelf(1, "");
    else if(play.checkCollision(u, v) && leftClick)
        sm->push(4, "client " + ip + " " + name);

    if(state==0)
    {
        if(dnReleased)
            state++;
        Universal::appendTextInput(ip, accept);
    } else if( state==1 )
    {
        if(upReleased)
            state--;
        Universal::appendTextInput(name, accept);
    }

    upPreviouslyPressed = upCurrentlyPressed;
    dnPreviouslyPressed = dnCurrentlyPressed;
}

void JoinState::draw(sf::RenderWindow& rw) const {
    back.draw(rw);
    play.draw(rw);

    if(state<2)
        rw.draw(highlight);

    rw.draw(ipLabel);
    rw.draw(ipInput);
    rw.draw(nameLabel);
    rw.draw(nameInput);
}

void JoinState::onDeactivate() {
    state = 0;
}
