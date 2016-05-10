#include "StateSubclass.h"

#include "StateManager.h"
#include "SplashState.h"
#include "MenuState.h"
#include "JoinState.h"
#include "HostState.h"
#include "GameState.h"
#include "Universal.h"

#include "mapgenerator.h"
#include <iostream>
#include <fstream>

EndState::EndState(StateManager* sm): State(sm), sm(sm)
{
    //sm->
}

void EndState::handleInput(int u, int v, const std::string& typed, sf::Event e)
{
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        sm->push(1,"");
        //sm->push(1,"");
        // popSelf(2, "");
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
                file<< '\n';

            }

			if (tilemap[i] == '4')
				tilemap[i] = 2;
			else if (tilemap[i] == '7' || tilemap[i] == '8')
				tilemap[i] = 3;

            file << tilemap[i];
        }
        file.close();
    }






}

void EndState::update(float dt)
{

}

void EndState::onActivate(const std::string& accept)
{
    isActive = true;
    tilemap = accept;
}

void EndState::draw(sf::RenderWindow& window) const
{
    sf::View end_view(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    sf::Text text;
    text.setString("You Win! Press space to end. Press S to save");
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/black_jack.ttf"))
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
