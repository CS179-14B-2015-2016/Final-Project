#ifndef OPTIONSSTATE_H_INCLUDED
#define OPTIONSSTATE_H_INCLUDED


#include "Button.h"

class StateManager;
class OptionsState: public State
{
private:
    StateManager* sm;
    sf::Font font;
    Button back;

    /*
    Button up;
    Button down;
    Button left;
    Button right;
    Button action;
    Button shift;
*/
    int UPKEY = 22,DOWNKEY = 18,LEFTKEY =0,RIGHTKEY = 3,SHIFTKEY =38,ACTIONKEY = 57;
    sf::RectangleShape highlight;
    sf::Text upInput, downInput, leftInput,rightInput,actionInput,shiftInput;
    sf::Text upLabel,downLabel,leftLabel,rightLabel,actionLabel,shiftLabel;
    std::string upt,downt,leftt,rightt,actiont,shiftt;

     uint8_t state;
public:
    OptionsState(StateManager* sm);
    void update(float dt);
    void draw(sf::RenderWindow&) const;
    void handleInput(int u, int v, const std::string& typed,sf::Event e);
    void onActivate();
    void onDeactivate();
};



#endif // OPTIONSSTATE_H_INCLUDED
