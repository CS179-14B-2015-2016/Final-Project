#ifndef STATESUBCLASS_H_INCLUDED
#define STATESUBCLASS_H_INCLUDED
#include "State.h"
#include "EntitySubclass.h"
#include "map.h"
#include <vector>
#include "mapgenerator.h"

class GameState: public State {
private:
    Map* map;
    StateManager* sm;
    bool isActive;
    Character player;
public:
    GameState(StateManager* sm);
    MapGenerator generator; //new
    bool isOver = false; //new
    sf::Clock gametime;
    const float GAME_DURATION = 1.0f;
    void handleInput();
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
};

//new
class EndState: public State {
private:
  //  Map* map;
    StateManager* sm;
    bool isActive;
  //  Character player;
public:
    EndState(StateManager* sm);
    std::string byimplication;
    void onActivate(std::string accept) override;
    void handleInput();
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
};
#endif // STATESUBCLASS_H_INCLUDED
