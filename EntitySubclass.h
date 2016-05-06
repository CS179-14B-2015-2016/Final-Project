#ifndef ENTITYSUBCLASS_H_INCLUDED
#define ENTITYSUBCLASS_H_INCLUDED
#include "Entity.h"
#include "map.h"

class Character: public Entity {
private:

protected:

    sf::Vector2f position;
    Movement direction;
    sf::Texture spriteSheet;
    int lastTic;

    int spriteDir;
    int spriteAction;
public:
    Character();
    void update(float dt);
    void draw(sf::RenderWindow& window) const;
    void setDirection(Movement m);
    void setFace(Movement m);
    int getFace();
    sf::Vector2i getIndexPosition();
    sf::Vector2f getPosition();
    sf::Vector2f setPosition(sf::Vector2f pos);
};

#endif // ENTITYSUBCLASS_H_INCLUDED
