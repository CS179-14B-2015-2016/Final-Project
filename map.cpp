#include "Map.h"
#include <iostream>

Map::Map(int w, int h)
{
    this->width = w;
    this->height = h;

    // Resize the tiles vector according to the width and height of the map.
    this->tiles.resize(h);
    for(int i = 0; i < h; i++)
    {
        this->tiles.at(i).resize(w);
    }

    // Initialize the tiles in the vector.
    for(int r = 0; r < this->tiles.size(); r++)
    {
        for(int c = 0; c < this->tiles.at(r).size(); c++)
        {
            this->tiles[r][c] = new Tile(((c * Tile::tileSize) + (Tile::tileSize / 2)), ((r * Tile::tileSize) + (Tile::tileSize / 2)), 1, false);
        }
    }
}

Map::Map(uint8_t** mapArray, size_t height, size_t width) :
	height(height),
	width(width)
{
	this->tiles.resize(height);

	for (int i = 0; i < height; i++)
	{
		this->tiles.at(i).resize(width);
	}

	// Initialize the tiles in the vector.
	for (int r = 0; r < this->tiles.size(); r++)
	{
		for (int c = 0; c < this->tiles.at(r).size(); c++)
		{
			this->tiles[r][c] = new Tile(((c * Tile::tileSize) + (Tile::tileSize / 2)), ((r * Tile::tileSize) + (Tile::tileSize / 2)), 1, false);
			int type = (int) mapArray[r][c];
			if (type == 4)
			{
				type = 2;
			}
			this->tiles[r][c]->setType(type);
		}
	}
}

Tile* Map::getTile(int row, int column)
{
    if(((row >= 0) && (row < this->height)) && ((column >= 0) && (column < this->width)))
    {
        return this->tiles[row][column];
    }

    else
    {
        return NULL;
    }

}

void Map::drawTiles(sf::RenderWindow* window)
{
    for(int r = 0; r < this->height; r++)
    {
        for(int c = 0; c < this->width; c++)
        {
            this->tiles[r][c]->draw(window);
        }
    }
}

uint8_t** Map::printTiles()
{
    uint8_t** passabilitymap;
    passabilitymap = new uint8_t*[height];

    for(uint8_t i = 0; i<height; i++)
    {
        passabilitymap[i] = new uint8_t[width];
    }

    for(int i = 0; i <height; i++)
    {
        for(int j = 0; j < width; j++)
        {

            passabilitymap[i][j] = tiles[i][j]->getType();

        }
    }

	/*
    for(int i = 0; i <height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            std::cout<<(int)passabilitymap[i][j];
        }
        std::cout<<std::endl;
    }
	*/
    return passabilitymap;


}

size_t Map::getWidth()
{
    return width;
}
size_t Map::getHeight()
{
    return height;
}
