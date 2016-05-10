#ifndef STATESUBCLASS_H_INCLUDED
#define STATESUBCLASS_H_INCLUDED
#include "State.h"
#include "map.h"
#include <vector>
#include "mapgenerator.h"


//new
class EndState: public State {
private:
  //  Map* map;
    StateManager* sm;
    bool isActive;
  //  Character player;
public:
    EndState(StateManager* sm);
    std::string tilemap;
    void onActivate(const std::string& accept) override;
    void handleInput(int u, int v, const std::string& typed, sf::Event e);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
};
#endif // STATESUBCLASS_H_INCLUDED
