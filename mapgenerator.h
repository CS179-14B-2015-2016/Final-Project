#ifndef MAPGENERATOR_H_INCLUDED
#define MAPGENERATOR_H_INCLUDED

#include "map.h"
#include <vector>
#include <SFML/Graphics.hpp>
class Leaf{
private:
    const int minLeafSize = 3;
public:
    Leaf() : row(0),column(0),width(0),height(0){};
    Leaf(int row, int column, int width, int height);
    sf::IntRect room;
    int row, column;
    int width, height;
    Leaf *leftchild;
    Leaf *rightchild;
    void split();
};

class MapGenerator{
private:
    const int maxLeafSize = 12;
    const int minLeafSize = 4;
public:
    std::vector<Leaf*> leafs;
    std::vector<Leaf*> rooms;
    std::vector<Leaf*> halls;
    std::vector<sf::Vector2i> objectTiles;
   // sf::Vector2i map[100][100];
    Map* generate(int w, int h, int depth);
    void split(Leaf *l,int current, int maxdepth);
    void createRooms(Map* map);
    void connectRooms(Map* map);
    };


#endif // MAPGENERATOR_H_INCLUDED
