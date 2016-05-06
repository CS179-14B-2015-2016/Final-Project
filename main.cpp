#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include "mapgenerator.h"

/*
using namespace std;
int main()
{


    //generator.generate();

    ifstream openfile("1.txt");

    sf::Texture tileTexture;
    sf::Sprite tiles;

    sf::Vector2i map[100][100];
    sf::Vector2i loadCounter = sf::Vector2i(0,0);
    if(openfile.is_open())
    {
            string tileLocation;
            openfile >> tileLocation;
            tileTexture.loadFromFile(tileLocation);
            tiles.setTexture(tileTexture);
            while(!openfile.eof())
            {
               string str;
               openfile >> str;
               char x = str[0];
               char y = str[2];
               if(!isdigit(x) || !isdigit(y))
               {
                   map[loadCounter.x][loadCounter.y] = sf::Vector2i(-1,-1);
               }
               else
               {
                   //-'0' to convert the char x to an integer
                   map[loadCounter.x][loadCounter.y] = sf::Vector2i(x - '0', y - '0');
               }
               if(openfile.peek() == '\n')
               {
                   loadCounter.x = 0;
                   loadCounter.y++;
               }
               else
               {
                    loadCounter.x++;
               }

            }
        loadCounter.y++;
    }
    sf::RenderWindow window(sf::VideoMode(1024,800,32), "Test");
    MapGenerator generator;
    Map* currentmap = generator.generate(1024/32,800/32,2);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            switch (event.type)
            {

            case sf::Event::Closed:
                window.close();
                break;
            }
        }
        window.clear();
        currentmap->drawTiles(&window);

        for(int i = 0; i < loadCounter.x; i++)
        {
            for(int j = 0; j < loadCounter.y; j++)
            {
                if(map[i][j].x != -1 && map[i][j].y != -1)
                {
                    tiles.setPosition(i * 32, j * 32);
                    tiles.setTextureRect(sf::IntRect(map[i][j].x * 32,  map[i][j].y * 32 , 32 ,32));
                   // window.draw(tiles);
                }
            }
        }
        window.display();
    }

}
*/
