#ifndef JOINSTATE_H_INCLUDED
#define JOINSTATE_H_INCLUDED
#include "Button.h"

class StateManager;
class JoinState: public State
{
private:
    StateManager* sm;
    Button back;
    Button play;
    sf::Font font;
    sf::Text ipLabel;
    sf::Text ipInput;
    sf::Text nameInput;
    sf::Text nameLabel;
    sf::RectangleShape highlight;
    std::string ip;
    std::string name;
    uint8_t state;
public:
    JoinState(StateManager* sm);
    void update(float dt);
    void draw(sf::RenderWindow&) const;
    void handleInput(int u, int v, const std::string& typed, sf::Event e);
    void onDeactivate();
};

#endif // JOINSTATE_H_INCLUDED
