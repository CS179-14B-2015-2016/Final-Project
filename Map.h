#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "Tile.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include <vector>

class Map {
	private:
		int width, height;
		std::vector< std::vector<Tile*> > tiles;

	public:
		Map(int w = 0, int h = 0);
		Tile* getTile(int row, int column);
		void drawTiles(sf::RenderWindow* window);
		uint8_t** printTiles();
		int getWidth();
		int getHeight();
};


#endif // MAP_H_INCLUDED
