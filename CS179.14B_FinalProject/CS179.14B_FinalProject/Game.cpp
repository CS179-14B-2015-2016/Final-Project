#include <iostream>
#include <algorithm>
#include <string>

#include "EntityManager.h"
#include "TextureLoader.h"
#include "Character.h"
#include "Tile.h"
#include "SObject.h"
#include "Game.h"

#define FPS 60.0f
#define SPF sf::seconds(1.0f/FPS)


using namespace std;

TextureLoader tl;
EntityManager em;
sf::RenderWindow* window;


void Init() {
	sf::Texture* maptex = tl.getTexture("Art/Maps/sample3.png");
	vector<sf::Vector2f> summon_loc;
	sf::Image map;
	if (map.loadFromFile("Art/Maps/sample3.png")) {
		for (int x = 0; x < map.getSize().x; x++) {
			for (int y = 0; y < map.getSize().y; y++) {
				const sf::Color temp = map.getPixel(x, y);
				if (temp == sf::Color::Black) {
					em.addMapTile(new  NormalTile(TILE_SIZE, sf::Vector2f(x*TILE_SIZE, y*TILE_SIZE), "Art/Tiles/Tar_tile_32.png"));
				}
				else if (temp == sf::Color::Yellow) {/*SObjects Spawn*/
					em.addSObject(new HealBarrel(SOBJECT_SIZE, sf::Vector2f(TILE_SIZE*x - SOBJECT_SIZE, TILE_SIZE*y - SOBJECT_SIZE), "Art/SObjects/HealBarrel.png"));
				}
				else if (temp == sf::Color::Blue) {/*Player Spawn*/
					summon_loc.push_back(sf::Vector2f(x*TILE_SIZE, y * TILE_SIZE));
				}
			}
		}
		window = new sf::RenderWindow(sf::VideoMode(GAME::WINDOW_WIDTH, GAME::WINDOW_HEIGHT), "Fight Me");
	}
	else {
		cout << "ERROR" << "\n";
	}

	if (!summon_loc.empty()) {
		em.setMain(new War(10, 7, 2, 7, 3, 10, 10, summon_loc[0], "Art/Characters/1.png"));
	}

	
}



int main() {
	sf::Clock clock;
	sf::Time lag = sf::seconds(0);
	Init();
	while (window->isOpen()) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			if (event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Left){
					em.handleMouse(1,*window);
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					em.handleMouse(2,*window);
				}
			}
		}
		//input
		em.handleInput();
		//update entities
		em.update(SPF.asSeconds());
		//collision detection
		em.logic(SPF.asSeconds());
		//render
		window->clear();
		em.render(*window);
		window->display();

		const auto elapsed = clock.getElapsedTime() + lag;
		if (elapsed < SPF) {
			sf::sleep(SPF-elapsed);
			lag = sf::seconds(0);
		}
		else {
			lag = elapsed - SPF;
		}
		clock.restart();
	}
	window->close();
	return 0;
}
