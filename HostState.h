#ifndef HOSTSTATE_H_INCLUDED
#define HOSTSTATE_H_INCLUDED
#include "Button.h"

class StateManager;
class HostState: public State
{
private:
    StateManager* sm;
    sf::Font font;
    Button back;
	Button play;
    sf::Text nameInput;
    sf::Text nameLabel;
	sf::Text modeInput;
	sf::Text modeLabel;
	sf::Text mapInput;
	sf::Text mapLabel;

	sf::RectangleShape highlight;

	int* playerCount;
	std::string* modes;
    std::string name;
	unsigned int modeIndex;

	std::string* mapOption;
	unsigned int mapIndex;

	int state;

public:
    HostState(StateManager* sm);
    void update(float dt);
    void draw(sf::RenderWindow&) const;
    void handleInput(int u, int v, const std::string& typed, sf::Event e);
};

#endif // HOSTSTATE_H_INCLUDED
