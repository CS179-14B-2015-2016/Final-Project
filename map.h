#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "Tile.h"
#include "SFML/Graphics/RenderWindow.hpp"
#include <vector>

class Map {
	private:
		size_t width, height;
		std::vector< std::vector<Tile*> > tiles;

	public:
		Map(int w = 0, int h = 0);
		Map(uint8_t** mapArray, size_t height, size_t width);
		Tile* getTile(int row, int column);
		void drawTiles(sf::RenderWindow* window);
		uint8_t** printTiles();
		size_t getWidth();
		size_t getHeight();
};


#endif // MAP_H_INCLUDED
