#include "StateSubclass.h"
#include "StateManager.h"
#include "mapgenerator.h"
#include <iostream>
#include <fstream>

EndState::EndState(StateManager* sm): sm(sm)
{
    //sm->
}

void EndState::handleInput()
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        exit(1);
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        std::ofstream file;
        file.open ("savedmap.txt");


       char *receivebuffer = new char[800];
       for(int i = 0; i < 800; i++)
        {
            if(i % 32 ==0 && i != 0)
            {
              std::cout<<std::endl;
              file<< '\n';

            }
             std::cout<<byimplication[i];
             file << byimplication[i];
        }
         std::cout<<std::endl;
         file.close();

    }






}

void EndState::update(float dt)
{

}

void EndState::onActivate(std::string accept) {
        isActive = true;
        byimplication = accept;
    }

void EndState::draw(sf::RenderWindow& window) const
{
    sf::View end_view(sf::FloatRect(0, 0, window.getSize().x/2, window.getSize().y/2));
    sf::Text text;
    text.setString("You Win! Press space to end. Press S to save");
    sf::Font font;
    if (!font.loadFromFile("assets/yuyuko.ttf"))
    {
        // error...
    }
    text.setFont(font); // font is a sf::Font

// set the character size
    text.setCharacterSize(24);

// set the color
    text.setColor(sf::Color::White);

// set the text style
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    text.setPosition(0,1);

    window.setView(end_view);
    window.draw(text);
}
