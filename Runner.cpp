#include "StateManager.h"
#include "StateSubclass.h"

int main() {
    StateManager sm;
    GameState gs(&sm);
    EndState es(&sm);
     sm.addState(&gs);
    sm.addState(&es);
    sm.push(0);

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Jewel Heist", sf::Style::Close);

    sf::Clock clock;
    float lag = 0;
    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type==sf::Event::Closed)
                window.close();
        }

        sm.handleInput();
        sm.update(spf);

        window.clear(sf::Color::Black);
        sm.draw(window);
        window.display();

        float timePassed = lag+clock.getElapsedTime().asSeconds();
        if(timePassed<spf) {
            float extra = spf-timePassed;
            sf::sleep(sf::seconds(extra));
            lag = 0;
        } else {
            lag = timePassed-spf;
        }
        clock.restart();
    }
    return 0;
}
