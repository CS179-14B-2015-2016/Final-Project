#ifndef STATE_H_INCLUDED
#define STATE_H_INCLUDED
#include "Universal.h"
#include<SFML/Graphics.hpp>

class StateManager;
class State {
protected:
    friend class StateManager;
    StateManager* sm;
    bool isActive;
    void popSelf(unsigned int level=0, std::string pass="");
public:
    virtual void onActivate(std::string accept) {
        isActive = true;
    }
    virtual void onDeactivate() {
        isActive = false;
    }

    virtual void handleInput() = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) const = 0;
};

#endif // STATE_H_INCLUDED
