
#include <iostream>
#include <algorithm>
#include <string>
#include <SFML/Network.hpp>
#include "EntityManager.h"
#include "TextureLoader.h"
#include "Character.h"
#include "Tile.h"
#include "SObject.h"
#include "Game.h"
#include "GameMessage.h"
#include <boost/asio.hpp>

#define FPS 60.0f
#define SPF sf::seconds(1.0f/FPS)


using namespace std;
using namespace GAME;
TextureLoader tl;
EntityManager* em;
sf::RenderWindow* window;

ID player_id;
bool Init(ID player_id, playerChar player) {
	vector<sf::Vector2f> summon_loc;
	cout << "Initializing " <<  player <<endl;
	sf::Image map;
	if (map.loadFromFile("Art/Maps/4.png")) {
		for (int x = 0; x < map.getSize().x; x++) {
			for (int y = 0; y < map.getSize().y; y++) {
				const sf::Color temp = map.getPixel(x, y);
				sf::Vector3i rgb((int)temp.r, (int)temp.g, (int)temp.b);
				if (rgb == NORMAL_RGB) {
					em->addMapTile(new  NormalTile(sf::Vector2f(x*TILE_SIZE, y*TILE_SIZE)));
				}
				else if (rgb == LAVA_RGB) {
					em->addMapTile(new  LavaTile(sf::Vector2f(x*TILE_SIZE, y*TILE_SIZE), LAVA_TIMER));
				}
				else if (rgb == TRAMPOLINE_RGB) {
					em->addMapTile(new  TrampolineTile(sf::Vector2f(x*TILE_SIZE, y*TILE_SIZE)));
				}
				else if (rgb == TAR_RGB) {
					em->addMapTile(new  TarTile(sf::Vector2f(x*TILE_SIZE, y*TILE_SIZE)));
				}
				else if (rgb == SOBJECT_RGB) {/*SObjects Spawn*/
					if (x%3 == 2) { // this is temporary
						em->addSObject(new ExplodingBarrel(sf::Vector2f(TILE_SIZE*x - SOBJECT_SIZE, TILE_SIZE*y - SOBJECT_SIZE)));
					} else if (x%3 == 1) {
						em->addSObject(new PoisonousBarrel(sf::Vector2f(TILE_SIZE*x - SOBJECT_SIZE, TILE_SIZE*y - SOBJECT_SIZE)));
					} else if (x%3 == 0) {
						em->addSObject(new HealBarrel(sf::Vector2f(TILE_SIZE*x - SOBJECT_SIZE, TILE_SIZE*y - SOBJECT_SIZE)));
					}
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
		switch (player) {
		case playerChar::WAR: {
			em->setMain(new War(10, 7, 2, 7, 3, 20, summon_loc[0], player_id));
			break;
		}
	
		case playerChar::PESTILENCE: {
			em->setMain(new Pestilence(7, 10, 2, 5, 5, 20, summon_loc[0], player_id));
			break;
		}
		case playerChar::FAMINE: {
			em->setMain(new Famine(2, 7, 10, 3, 7, 20, summon_loc[0], player_id));
			break;
		}
		case playerChar::DEATH: {
			em->setMain(new Death(10, 2, 7, 3, 7, 20, summon_loc[0], player_id));
			break;
		}
		case playerChar::MATTHEW: {
			em->setMain(new Matthew(2, 7, 10, 3, 7, 20, summon_loc[0], player_id));
			break;
		}
		case playerChar::MARK: {
			em->setMain(new Mark(10, 7, 2, 7, 3, 20, summon_loc[0], player_id));
			break;
		}
		case playerChar::LUKE: {
			em->setMain(new Luke(10, 7, 2, 7, 3, 20, summon_loc[0], player_id));
			break;
		}
		case playerChar::JOHN: {
			em->setMain(new John(7, 10, 2, 7, 3, 20, summon_loc[0], player_id));
			break;
		}
		}
	}
	cout << "Connected. Client id: " << player_id << endl;

	return true;
}

int main() {
	sf::Font Font;
	bool isLoggedIn = false;
	bool charSelected = false;
	playerChar player = playerChar::PESTILENCE;
	if (!Font.loadFromFile("font.ttf")) {
		std::cout << "Can't load font" << std::endl;
	}
	sf::RenderWindow loginScreen;
	sf::RenderWindow characterSelection;
	string ip;
	string tempIP;
	sf::Text displayIP(tempIP, Font, 50);
	//displayIP.setPosition(200, 150);
	displayIP.setColor(sf::Color(44, 127, 255));
	sf::FloatRect textRect = displayIP.getLocalBounds();

	loginScreen.create(sf::VideoMode(400, 60), "ENTER IP ADDRESS:");
	//loginScreen.setKeyRepeatEnabled(false);

	while (loginScreen.isOpen()) {
		sf::Event loginEvent;
		while (loginScreen.pollEvent(loginEvent)) {
			if (loginEvent.type == sf::Event::Closed) {
				loginScreen.close();
			}
			if (loginEvent.type == sf::Event::TextEntered) {
				if (loginEvent.text.unicode >= 33 && loginEvent.text.unicode <= 126) {
					tempIP += static_cast<char>(loginEvent.text.unicode);
				}
				displayIP.setString(tempIP);

			}
			if (loginEvent.type == sf::Event::KeyPressed) {
				if (loginEvent.key.code == sf::Keyboard::Return)
				{
					ip = tempIP;
					tempIP = "";
					isLoggedIn = true;
					loginScreen.close();
				}
				if (loginEvent.key.code == sf::Keyboard::Space)
				{
					tempIP += " ";
					displayIP.setString(tempIP);
				}
				if (loginEvent.key.code == sf::Keyboard::BackSpace)
				{
					if (tempIP.length() >= 1) {
						tempIP.erase(tempIP.length() - 1);
					}
					displayIP.setString(tempIP);
				}
			}
		}
		displayIP.setPosition(20, 0);
		loginScreen.draw(displayIP);
		loginScreen.display();
		loginScreen.clear();
	}
	//code starts here for char selection
	characterSelection.create(sf::VideoMode(400, 200), "SELECT CHARACTER");
	while (characterSelection.isOpen() && isLoggedIn && !charSelected) {
		sf::Event characterEvent;
		sf::Texture texture;
		sf::Sprite sprite;

		if (!texture.loadFromFile("Art/Characters/charselection.png")) {
			std::cout << "Ooops" << std::endl;
		}
		sprite.setTexture(texture);
		sprite.setPosition(sf::Vector2f(0, 0));
		while (characterSelection.pollEvent(characterEvent)) {
			if (characterEvent.type == sf::Event::Closed) {
				characterSelection.close();
			}
			if (characterEvent.key.code == sf::Keyboard::BackSpace) {
				characterSelection.close();
			}
			if (characterEvent.type == sf::Event::MouseButtonPressed)
			{
				if (characterEvent.mouseButton.button == sf::Mouse::Left)
				{

					if (characterEvent.mouseButton.y >= 0 && characterEvent.mouseButton.y < 100) {
						if (characterEvent.mouseButton.x >= 0 && characterEvent.mouseButton.x < 100) {
							//character1
							std::cout << "WAR" << std::endl;
							player = playerChar::WAR;
							charSelected = true;
							break;
						}
						if (characterEvent.mouseButton.x >= 100 && characterEvent.mouseButton.x < 200) {
							std::cout << "FAMINE" << std::endl;
							player = playerChar::FAMINE;
							charSelected = true;
							break;
						}
						if (characterEvent.mouseButton.x >= 200 && characterEvent.mouseButton.x < 300) {
							std::cout << "PESTILENCE" << std::endl;
							player = playerChar::PESTILENCE;
							charSelected = true; 
							break;
						}
						if (characterEvent.mouseButton.x >= 300 && characterEvent.mouseButton.x < 400) {
							std::cout << "DEATH" << std::endl;
							player = playerChar::DEATH;
							charSelected = true; 
							break;
						}
					}
					if (characterEvent.mouseButton.y >= 100 && characterEvent.mouseButton.y < 200) {
						if (characterEvent.mouseButton.x >= 0 && characterEvent.mouseButton.x < 100) {
							std::cout << "MATTHEW" << std::endl;
							player = playerChar::MATTHEW;
							charSelected = true;
							break;
						}
						if (characterEvent.mouseButton.x >= 100 && characterEvent.mouseButton.x < 200) {
							std::cout << "MARK" << std::endl;
							player = playerChar::MARK;
							charSelected = true; 
							break;
						}
						if (characterEvent.mouseButton.x >= 200 && characterEvent.mouseButton.x < 300) {
							std::cout << "LUKE" << std::endl;
							player = playerChar::LUKE;
							charSelected = true;
							break;
						}
						if (characterEvent.mouseButton.x >= 300 && characterEvent.mouseButton.x < 400) {
							std::cout << "JOHN" << std::endl;
							player = playerChar::JOHN;
							charSelected = true;
							break;
						}
					}

				}
			}
		}

		characterSelection.draw(sprite);
		characterSelection.display();
		characterSelection.clear();

	}

	characterSelection.close();
	sf::Clock clock;
	sf::Time lag = sf::seconds(0);
	sf::UdpSocket socket;
	const unsigned short port = 8080;

	const unsigned short listen_port = port + 1;
	sf::IpAddress server_address(ip);
	socket.bind(listen_port);
	std::cout << "Connecting to Server...";
	{
		uint8_t buffer[sizeof(Message) + sizeof(ID)];
		auto msg = reinterpret_cast<Message*>(buffer);
		msg->type = MessageType::Connect;
		msg->size = 0;
		if (socket.send(buffer, sizeof(Message), server_address, port) != sf::Socket::Done) {
			cerr << "Cannot connect to server." << endl;
			system("pause");
			return -1;
		}


		size_t recv_size;
		sf::IpAddress recv_addr;
		unsigned short  recv_port;
		socket.setBlocking(true);
		if (socket.receive(buffer, sizeof(buffer), recv_size, recv_addr, recv_port) != sf::Socket::Done) {
			cerr << "Cannot receive from server." << endl;
			system("pause");
			return -1;
		}
		assert(recv_size == sizeof(buffer));
		assert(msg->type == MessageType::Connect);
		assert(msg->size == sizeof(ID));
		player_id = *reinterpret_cast<ID*>(msg->data);
	}
	std::cout << "Connected to Server!" << endl;
	socket.setBlocking(false);
	em = new EntityManager(&socket, ip.c_str(), port);
	bool success = Init(player_id, player);
	while (window->isOpen() && success && isLoggedIn && charSelected) {
		sf::Event event;
		while (window->pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window->close();
			if (event.type == sf::Event::MouseButtonPressed) {
				if (event.mouseButton.button == sf::Mouse::Left) {
					em->handleMouse(1, *window);
				}
				if (event.mouseButton.button == sf::Mouse::Right) {
					em->handleMouse(2, *window);
				}
			}
		}
		//input
		em->handleInput();
		//update entities
		em->update(SPF.asSeconds());
		//collision detection
		em->logic(SPF.asSeconds());
		//render
		window->clear();
		em->render(*window);
		window->display();

		const auto elapsed = clock.getElapsedTime() + lag;
		if (elapsed < SPF) {
			sf::sleep(SPF - elapsed);
			lag = sf::seconds(0);
		}
		else {
			lag = elapsed - SPF;
		}
		clock.restart();
	}
	window->close();
	if (!success) {
		cout << "Failed to connect to server" << endl;
	}
	else {
		delete em;
		delete window;
	}

	return 0;
}





