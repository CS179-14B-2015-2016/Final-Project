#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include "StateManager.h"
#include "SplashState.h"

SplashState::SplashState(StateManager* sm):
State(sm),
sm(sm)
{
    this->sm = sm;
    isFadingIn = false;
    isOnPause = false;
    isFadingOut = false;
}

void SplashState::update(float dt)
{
    if(isFadingIn || isFadingOut || isOnPause)
        timeCount += dt;
    if(isFadingIn)
    {
        if(timeCount>=1)
        {
            timeCount -= 1;
            logoAlpha = 255;
            isFadingIn = false;
            isOnPause = true;
            isFadingOut = false;
        }
        else
        {
            int newAlpha = logoAlpha+(int)(255*dt);
            logoAlpha = std::min(newAlpha, 255);
        }
    }
    else if(isOnPause)
    {
        if(timeCount>=1)
        {
            timeCount -= 1;
            logoAlpha -= (255*timeCount);
            isFadingIn = false;
            isFadingOut = true;
            isOnPause = false;
        }
    }
    else if(isFadingOut)
    {
        if(timeCount>=1)
        {
            logoAlpha = 0;
            isFadingOut = false;
            sm->push(1);
        }
        else
        {
            int newAlpha = logoAlpha-(int)(255*dt);
            logoAlpha = std::max(newAlpha, 0);
        }
    }
}

void SplashState::draw(sf::RenderWindow& window) const
{
    sf::Texture splash_logo;
    splash_logo.loadFromFile("assets/sprites/chandelier-logo.png", sf::IntRect(0, 0, 0, 0));
    sf::Sprite splash_sprite;
    splash_sprite.setTexture(splash_logo);
    int alpha = logoAlpha;
    splash_sprite.setColor(sf::Color(255, 255, 255, alpha));
    splash_sprite.setPosition(250, 280);
    window.draw(splash_sprite);
}

void SplashState::onActivate(const std::string& accept)
{
    isActive = true;
    isFadingIn = true;
    timeCount = 0;
    logoAlpha = 0;
}

void SplashState::onDeactivate()
{
    isActive = false;
    isFadingIn = false;
    isOnPause = false;
    isFadingOut = false;
    timeCount = 0;
    logoAlpha = 0;
}

void SplashState::handleInput(int u, int v, const std::string& typed, sf::Event e)
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sm->push(1);
    }
}
