#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED
#include "Universal.h"
#include <SFML/Graphics.hpp>
#include <iostream>

class StateManager;
class State {
protected:
    friend class StateManager;
    StateManager* sm;
    bool isActive;
    void popSelf(unsigned int level=0, const std::string& pass="");
public:
    State(StateManager* sm): sm(sm) {}
	virtual ~State() {}
    virtual void onActivate(const std::string& accept) {
        isActive = true;
    }
    virtual void onDeactivate() {
        isActive = false;
    }

    virtual void handleInput(int u, int v, const std::string& typed, sf::Event e) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};

#endif // STATE_H_INCLUDED
