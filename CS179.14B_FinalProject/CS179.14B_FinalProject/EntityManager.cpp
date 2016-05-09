#include "EntityManager.h"
#include "../GameMessage.h"

#include <cassert>
#include <algorithm>
#include <fstream>

void EntityManager::addMapTile(Tile* t) {
	map.push_back(t);
}

void EntityManager::addSObject(SObject* so) {
	sobjects.push_back(so);
}

void EntityManager::setMain(Character* p, const sf::Vector2f& start_pos) {
	main_player = std::move(p);
	main_player->setPosition(start_pos);
	std::ifstream death_counter_i("death.txt");
	if (death_counter_i.is_open()) {
		int num_deaths = 0;
		death_counter_i >> num_deaths;
		main_player->set_num_deaths(num_deaths);
	}
}

void EntityManager::handleInput() {
	main_player->handleInput();
}

void EntityManager::handleMouse(int key, sf::RenderWindow& g) {
	int command = main_player->handleMouse(key, g);
	{
		uint8_t buffer[sizeof(Message) + sizeof(AttackMessage)];
		auto message = reinterpret_cast<Message*>(buffer);
		auto data = reinterpret_cast<AttackMessage*>(message->data);
		message->origin = main_player->getId();
		message->message_type = MessageType::Broadcast;
		message->broadcast_type = BroadcastType::Attack;
		message->size = sizeof(AttackMessage);
		data->attack = command;
		if (socket->send(buffer, sizeof(buffer), address, port) == !sf::Socket::Done) {
			cout << "Not sending data" << endl;
		}
	}
}

void EntityManager::update(float dt) {
	{
		sf::Packet packet;
		sf::IpAddress remote_address;
		unsigned short remote_port;
		if (socket->receive(packet, remote_address, remote_port) == sf::Socket::Done) {
			assert(packet.getDataSize() >= sizeof(Message));
			auto message = reinterpret_cast<const Message*>(packet.getData());
			switch (message->broadcast_type) {
				case BroadcastType::Status: {
					auto data = reinterpret_cast<const StatusMessage*>(message->data);
					auto it = std::find_if(other_players.begin(), other_players.end(), [message](const Character *p) {
						return p->getId() == message->origin;
					});
					if (it == other_players.end()) {
						switch (data->unit) {
							case playerChar::WAR: {
								other_players.emplace_back(new War(message->origin));
								break;
							}
							case playerChar::PESTILENCE: {
								other_players.emplace_back(new Pestilence(message->origin));
								break;
							}
							case playerChar::FAMINE: {
								other_players.emplace_back(new Famine(message->origin));
								break;
							}
							case playerChar::DEATH: {
								other_players.emplace_back(new Death(message->origin));
								break;
							}
							case playerChar::MATTHEW: {
								other_players.emplace_back(new Matthew(message->origin));
								break;
							}
							case playerChar::MARK: {
								other_players.emplace_back(new Mark(message->origin));
								break;
							}
							case playerChar::LUKE: {
								other_players.emplace_back(new Luke(message->origin));
								break;
							}
							case playerChar::JOHN: {
								other_players.emplace_back(new John(message->origin));
								break;
							}
						}
					} else {
						(*it)->update(sf::Vector2f(data->px, data->py), data->hp, data->num_deaths);
					}
					break;
				}
				case BroadcastType::Attack: {
					auto data = reinterpret_cast<const AttackMessage*>(message->data);
					auto it = std::find_if(other_players.begin(), other_players.end(), [message](const Character *p) {
						return p->getId() == message->origin;
					});
					int command = data->attack;
					switch (command) {
						case 1: {
							(*it)->get_weapon()->attack((*it)->get_weapon()->get_left_attack());
							break;
						}
						case 2: {
							(*it)->get_weapon()->attack((*it)->get_weapon()->get_right_attack());
							break;
						}
					}
					break;
				}
				case BroadcastType::Hit: {
					auto it = std::find_if(other_players.begin(), other_players.end(), [message](const Character *p) {
						return p->getId() == message->origin;
					});
					(*it)->takeDamage(1);
					break;
				}
			}
		}
	}

	main_player->update(dt);
	for (auto e : other_players) {
		e->update(dt);
	}
	for (auto e : map) {
		e->update(dt);
	}
	for (auto e : sobjects) {
		e->update(dt);
	}

	{
		uint8_t buffer[sizeof(Message) + sizeof(StatusMessage)];
		auto message = reinterpret_cast<Message*>(buffer);
		auto data = reinterpret_cast<StatusMessage*>(message->data);
		message->origin = main_player->getId();
		message->message_type = MessageType::Broadcast;
		message->broadcast_type = BroadcastType::Status;
		message->size = sizeof(StatusMessage);
		data->px = main_player->getPosition().x;
		data->py = main_player->getPosition().y;
		// data->vx = main_player->getVel().x;
		// data->vy = main_player->getVel().y;
		// data->face = main_player->getFace();
		data->hp = main_player->getHealth();
		data->num_deaths = main_player->get_num_deaths();
		// data->order = -1; // CHANGE THIS
		data->unit = main_player->getType();
		if (socket->send(buffer, sizeof(buffer), address, port) == !sf::Socket::Done) {
			std::cout << "Not sending data" << std::endl;
		}
	}

	if (main_player->is_dead()) {
		main_player->revive();
		death_counter_o.open("death.txt", std::ios::trunc);
		death_counter_o << main_player->get_num_deaths();
		death_counter_o.close();
	}
}

void EntityManager::logic(float dt) {
	resolveCollisions(dt);
}

void EntityManager::render(sf::RenderTarget& g) {
	sf::RectangleShape x(sf::Vector2f(2000, 2000));
	x.setFillColor(sf::Color::White);
	g.draw(x);
	
	main_player->render(g);
	view.setCenter(main_player->getPosition());
	g.setView(view);
	
	for (auto e : map) {
		e->render(g);
	}
	for (auto e : other_players) {
		e->render(g);
	}
	for (auto e : sobjects) {
		e->render(g);
	}
}

bool EntityManager::has_collided(Tile* t, Character* p) {
	sf::Rect<float> collision;
	
	if (t->bounds().intersects(p->bounds(), collision)) {
		if (collision.height < collision.width) {
			if (collision.top > p->bounds().top) {
				p->move(sf::Vector2f(0, -collision.height));
				p->resetGravity();
				return true;
			} else {
				p->move(sf::Vector2f(0, collision.height));
				p->hit_head();
			}
		} else {
			if (collision.left > p->bounds().left) {
				p->move(sf::Vector2f(-collision.width, 0));
			} else {
				p->move(sf::Vector2f(collision.width, 0));
			}
			
		}
	}
	return false;
}

bool EntityManager::has_collided(SObject* s, Character* p) {
	sf::Rect<float> collision;
	
	if (s->bounds().intersects(p->bounds(), collision)) {
		if (collision.height < collision.width) {
			if (collision.top > p->bounds().top) {
				p->move(sf::Vector2f(0, -collision.height));
				p->resetGravity();
			} else {
				p->move(sf::Vector2f(0, collision.height));
				p->hit_head();
			}
		} else {
			if (collision.left > p->bounds().left) {
				p->move(sf::Vector2f(-collision.width, 0));
			} else {
				p->move(sf::Vector2f(collision.width, 0));
			}
		}
		return true;
	}
	return false;
}

bool EntityManager::has_collided(Weapon* w, Character* c) {
	if (w->bounds().intersects(c->bounds())) {
		// c->takeDamage(w->power);
		return true;
	}
	return false;
}

bool EntityManager::has_collided(Weapon* w, SObject* t) {
	if (t->bounds().intersects(w->bounds())) {
		t->collide(w);
		return true;
	}
	return false;
}

void EntityManager::resolveCollisions(float dt) {
	//checks map collision
	for (auto tile : map) {
		if (has_collided(tile, main_player)) {
			tile->collide(main_player);
		}
		for (auto player : other_players) {
			if (has_collided(tile, player)) {
				tile->collide(player);
			}
		}
	}
	
	//checks sobject collision
	for (int i = 0; i < sobjects.size(); i++) {
		if (has_collided(sobjects[i], main_player)) {}
		if (main_player->get_weapon()->is_attacking() && has_collided(main_player->get_weapon(), sobjects[i])) {
			sobjects[i]->damage();
			if (sobjects[i]->can_be_destroyed()) {
				sobjects[i]->collide(main_player);
				sobjects.erase(sobjects.begin() + i--);
			}
		}
	}
	
	for (auto u : other_players) {
		if (u->get_weapon()->is_attacking() && has_collided(u->get_weapon(), main_player)) {
			u->get_weapon()->hit();
			main_player->takeDamage(1);
		}
		/* if (main_player->get_weapon()->is_attacking() && has_collided(main_player->get_weapon(), u)) {
			main_player->get_weapon()->hit();
			u->takeDamage(1);
		} */
	}
}
