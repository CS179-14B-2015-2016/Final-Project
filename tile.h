#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED


#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class Tile {
	private:
		float cx, cy;
		int type;
		bool passability;

		sf::RectangleShape shape;

	public:
		static constexpr float tileSize = 32;

		Tile(float cx, float cy, int type, bool passability);

		int getType();
		void setType(int type);

		bool isPassable();

		void setColor(sf::Color color);
		void draw(sf::RenderWindow* window);
		float getX();
		float getY();
};


#endif // TILE_H_INCLUDED
