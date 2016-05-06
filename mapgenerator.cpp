#include "mapgenerator.h"

#include "SFML/Graphics/Color.hpp"
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include <iostream>
#include <chrono>
#include <math.h>
/*
    Should have been a non rand randomizer but could not make it to work the way we wanted to
*/
int chance(int x, int y)
{
    int ran =(int) (rand() % y) + x;
    return ran;

    /*
    unsigned seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    // std::mt19937::result_type seed = time(0);
    std::mt19937 random(seed);
    std::uniform_int_distribution<int> distribution(x, y);

    // std::random_device random;
    //std::uniform_int_distribution<int> distribution(x,y);
    // int r = distribution(random);

    //int r = (int)std::bind(std::uniform_int_distribution<int>(x,y),std::mt19937(seed));
    return distribution(random);
    */
}
/*
   Leaf constructor
*/
Leaf::Leaf(int row, int column, int width, int height)
{
    this->row = row;
    this->column = column;
    this->width = width;
    this->height = height;
}
/*
    Splits the map randomly, given the total width and height, and pushes it to a vector of Leafs/rooms.
*/
void MapGenerator::split(Leaf *leaf,int current, int maxdepth)
{

    leaf->leftchild = new Leaf();
    leaf->rightchild = new Leaf();

    // if horizontal cut or vertical cut
    bool splithorizon = ((rand()%2 +1) == 1);
    //if width > height by 25%, cut vertically
    if (leaf->width > leaf->height && leaf->width / leaf->height >= 1.25)
        splithorizon = false;
    else if (leaf->height > leaf->width && leaf->height / leaf->width >= 1.25)
        splithorizon = true;

    if(splithorizon)
    {
        int min = (int)(0.4 * leaf->height);
        if(min >= minLeafSize)
        {
            int min = (int)(0.3 * leaf->height);
            int max = (int)(0.6 * leaf->height);
            int rowLine = ((rand() % (max - min + 1)) + min); //max row
            leaf->leftchild->row = leaf->row;
            leaf->leftchild->column = leaf->column;
            leaf->leftchild->width = leaf->width;
            leaf->leftchild->height = (((leaf->leftchild->row + rowLine) + 1) - leaf->leftchild->row);
            leaf->rightchild->row = (leaf->leftchild->row + (rowLine + 1));
            leaf->rightchild->column = leaf->column;
            leaf->rightchild->width = leaf->width;
            leaf->rightchild->height = (leaf->height - leaf->leftchild->height);
        }
        else{
        }
    }
    else
    {
        int min = (int)(0.4 * leaf->width);
        if(min >= minLeafSize)
        {

            int min = (int)(0.3 * leaf->width);
            int max = (int)(0.6 * leaf->width);
            int colLine = ((rand() % (max - min + 1)) + min); // max column
            leaf->leftchild->row = leaf->row;
            leaf->leftchild->column = leaf->column;
            leaf->leftchild->width = (((leaf->leftchild->column + colLine) + 1) - leaf->leftchild->column);
            leaf->leftchild->height = leaf->height;
            leaf->rightchild->row = leaf->row;
            leaf->rightchild->column = (leaf->leftchild->column + (colLine + 1));
            leaf->rightchild->width = (leaf->width - leaf->leftchild->width);
            leaf->rightchild->height = leaf->height;
        }
        else{
        }
    }

    //recurse if current depth is not equal to max depth
    if(current < maxdepth)
    {
        this->split(leaf->leftchild, current+1, maxdepth);
        this->split(leaf->rightchild, current+1, maxdepth);
    }
    else
    {
        this->rooms.push_back(leaf->leftchild);
        this->rooms.push_back(leaf->rightchild);
    }

}
/*
    Map Generator main method. Generates a map using the methods here.
*/
Map* MapGenerator::generate(int w, int h, int depth)
{

    srand(time(NULL));

    Map* finalmap = new Map(w, h);

    // create main leaf
    Leaf* mainleaf = new Leaf();

    mainleaf->row = 0;
    mainleaf->column = 0;
    mainleaf->width = w;
    mainleaf->height = h;

    int currentDepth = 0;
    if(currentDepth < depth)
    {
        this->split(mainleaf, currentDepth, depth);
    }
    //clean up dead leafs

    rooms.erase(std::remove_if(rooms.begin(),rooms.end(),
                               [](Leaf* i)
    {
        return i->column ==0 && i->row == 0 && i->width == 0 && i->height == 0;
    }),rooms.end());

    this->createRooms(finalmap);
    this->connectRooms(finalmap);

    return finalmap;


}
/*
    Creates the halls that connect each leaf(room) to their neighbors
*/
void MapGenerator::connectRooms(Map* map)
{

    //CREATE WALLS (PUTS WHITE TILES AROUND LEAF BORDER)
    for(int i = 0; i < this->rooms.size(); i++)
    {
        Leaf* leaf = this->rooms[i];
        std::cout<<leaf->column<<" " << leaf->row << " " << leaf->width << " " << leaf->height << std::endl;
        int initialwidth = leaf->width;
        int initialheight = leaf->height;
        sf::Color color = sf::Color::White;
        //build walls around rooms
        for(int r = leaf->row; r < (leaf->row + leaf->height); r++)
        {
            for(int c = leaf->column; c < (leaf->column + leaf->width); c++)
            {
                if(r == leaf->row)
                {
                    map->getTile(r, c)->setType(1);
                    map->getTile(r, c)->setColor(color) ;

                }
                if(c == leaf->column)
                {
                    map->getTile(r, c)->setType(1);
                    map->getTile(r, c)->setColor(color);

                }
                if(r == leaf->row + leaf->height-1)
                {
                    map->getTile(r, c)->setType(1);
                    map->getTile(r, c)->setColor(color);

                }
                if(c == leaf->column + leaf->width -1)
                {
                    map->getTile(r, c)->setType(1);
                    map->getTile(r, c)->setColor(color);

                }

                if(map->getTile(r+1,c)!= NULL && map->getTile(r+1,c)->getType() != 0)
                {
                    map->getTile(r+1, c)->setType(1);
                    map->getTile(r+1, c)->setColor(color);

                }

                if(map->getTile(r-1,c)!= NULL && map->getTile(r-1,c)->getType() != 0)
                {
                    map->getTile(r-1, c)->setType(1);
                    map->getTile(r-1, c)->setColor(color);

                }
                if(map->getTile(r,c+1)!= NULL && map->getTile(r,c+1)->getType() != 0)
                {
                    map->getTile(r, c+1)->setType(1);
                    map->getTile(r, c+1)->setColor(color);

                }
                if(map->getTile(r,c-1)!= NULL && map->getTile(r,c-1)->getType() != 0)
                {
                    map->getTile(r, c-1)->setType(1);
                    map->getTile(r, c-1)->setColor(color);

                }

            }

        }


    }

// test
    for(int i = 0; i < this->rooms.size(); i++)
    {
        for(int j = 0; j < this->rooms.size(); j++)
        {
            std::cout<<"I: "<<i<<" J: "<<j<<std::endl;


            Leaf* room1 = rooms[i];
            Leaf* room2 = rooms[j];
            if(i == j)
            {
                std::cout<<"Continued i == j" <<std::endl;
                continue;
            }



            bool left = (room1->column - room2->column) < 0;
            bool top = (room1->row - room2->row) < 0;
            //if room 1 is to the left of room2 AND on top, or vice versa, not adjacent
            if(left && top || !left&& !top)
            {
                continue;
            }
            //if room1 is to the left of room2
            else if(left)
            {
                std::cout<<"left" << std::endl;
                int hallrow = (int)(rand()%(room1->height >= room2->height ? room2->height-2:room1->height-2) + (room1->row+1));
                Leaf* hall = new Leaf(hallrow,room1->column + room1->width - 1,3,1);
                if(hall->column +3 <  map->getWidth() && hall->row+1 <map->getHeight())
                {
                    if(map->getTile(hall->row+1,hall->column+3)->getType() != 0)
                    {
                        hall = new Leaf(hallrow,room1->column + room1->width - 1,3,1);

                    }
                    this->halls.push_back(hall);
                }
            }
            //if room1 is at the top of room2
            else if (top)
            {
                std::cout<<"top" << std::endl;
                int hallcolumn = (int)(rand()%(room1->width >= room2->width ? room2->width-2:room1->width-2) + (room1->column+1));
                std::cout<<"top" << std::endl;
                Leaf* hall = new Leaf(room1->row + room1->height - 1,hallcolumn,1,3);
                std::cout<<"top" << std::endl;
                if(hall->column +1 < map->getWidth() && hall->row+3 <map->getHeight())
                {


                    if(map->getTile(hall->row+3,hall->column+1)->getType() != 0)
                    {
                        hall = new Leaf(room1->row + room1->height - 1,hallcolumn,1,3);
                        std::cout<<"top" << std::endl;

                    }
                    this->halls.push_back(hall);
                }

            }
            //if room1 is to the right of room2
            else if (!left)
            {
                std::cout<<"!left" << std::endl;
                int hallrow = (int)(rand()%(room2->height >= room1->height ? room1->height-2:room2->height-2) + (room2->row+1));
                Leaf* hall = new Leaf(hallrow,room2->column + room2->width - 1,3,1);
                if(map->getTile(hall->row+1,hall->column+3)->getType() != 0)
                {
                    hall = new Leaf(hallrow,room1->column + room1->width - 1,3,1);

                }
                this->halls.push_back(hall);

            }
            //if room1 is below room2
            else if(!top)
            {
                std::cout<<"!top" << std::endl;
                int hallcolumn = (int)(rand()%(room2->width >= room1->width ? room1->width-2:room2->width-2) + (room2->column+1));
                Leaf* hall = new Leaf(room2->row + room2->height - 1,hallcolumn,1,3);
                if(map->getTile(hall->row+3,hall->column+1)->getType() != 0)
                {
                    hall = new Leaf(room1->row + room1->height - 1,hallcolumn,1,3);

                }
                this->halls.push_back(hall);

            }
            else {}

        }
    }
//color halls and set type
    for(int i = 0; i < this->halls.size(); i++)
    {
        Leaf* leaf = this->halls[i];
        //color each tile in the room
        sf::Color color = sf::Color::Black;
        for(int r = leaf->row; r < (leaf->row + leaf->height); r++)
        {
            for(int c = leaf->column; c < (leaf->column + leaf->width); c++)
            {
                map->getTile(r,c)->setType(3);
                map->getTile(r,c)->setColor(color);
            }
        }
    }


// RANDOM OBJECT GENERATION PER ROOM
    for(int i = 0; i < this->rooms.size(); i++)
    {
        Leaf* leaf = this->rooms[i];
        sf::Color color = sf::Color::Yellow;
        //LOOP UNTIL AN OBJECT IS CREATED IN A ROOM
        while(true)
        {
            int XPOS = rand() % (1 + leaf->width-2) + leaf->column;
            int YPOS = rand() % (1 + leaf->height-2) + leaf->row;
            if(map->getTile(YPOS,XPOS)-> getType() == 0)
            {
                //CHECK SO THAT THE OBJECTS DO NOT BLOCK ANY HALLS TO OTHER ROOMS
                if(map->getTile(YPOS+1,XPOS)->getType() != 3 &&
                        map->getTile(YPOS-1,XPOS)->getType() != 3 &&
                        map->getTile(YPOS,XPOS+1)->getType() != 3 &&
                        map->getTile(YPOS,XPOS-1)->getType() != 3)
                {
                    map->getTile(YPOS,XPOS) ->setType(2);
                    map->getTile(YPOS,XPOS) -> setColor(color);
                    objectTiles.push_back(sf::Vector2i(XPOS,YPOS));

                    break;
                }


            }
        }
    }

    /*
    //PICK JEWEL OUT OF OBJECTS
    int jewelIndex = rand() % (objectTiles.size());

    sf::Vector2i jewelPos = objectTiles[jewelIndex];
    map->getTile(jewelPos.y,jewelPos.x)->setColor(sf::Color::Blue);
    std::cout<<"JEWEL INDEX: " << jewelPos.x << " "<<jewelPos.y <<std::endl;
    */

}

/*
    Maps each leaf of a certain width and height to the tile map given the position of the leaf
*/
void MapGenerator::createRooms(Map* map)
{
    for(int i = 0; i < this->rooms.size(); i++)
    {

        Leaf* leaf = this->rooms[i];
        int initialwidth = leaf->width;
        int initialheight = leaf->height;
        int row = leaf->row;
        int column = leaf->column;
        //if the leaf is not a 'dead' leaf
        if(initialwidth != 0 || initialheight != 0)
        {
            sf::Color color = sf::Color::Black;
            for(int r = leaf->row; r < (leaf->row + leaf->height); r++)
            {
                for(int c = leaf->column; c < (leaf->column + leaf->width); c++)
                {
                    map->getTile(r,c)->setType(0);
                    map->getTile(r, c)->setColor(color);
                }
            }



        }
    }

}
