#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED
#include <vector>
#include <iostream>
#include "State.h"

class StateManager {
private:
    std::vector<State*> states;
    std::vector<State*> state_stack;
public:
    StateManager() {}
    void addState(State* state);
    void push(unsigned int id, const std::string& pass="");
    void pop(unsigned int lvl=1, const std::string& pass="");
    void handleInput(int u, int v, const std::string& typed, sf::Event e);
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
};


#endif // STATEMANAGER_H_INCLUDED
