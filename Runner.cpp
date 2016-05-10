#include "StateManager.h"
#include "SplashState.h"
#include "MenuState.h"
#include "JoinState.h"
#include "HostState.h"
#include "GameState.h"
#include "Universal.h"
#include "StateSubclass.h"
#include "OptionsState.h"

int main() {
    srand(time(NULL));
    StateManager sm;
    SplashState s(&sm);		// 0
    MenuState ms(&sm);		// 1
    JoinState js(&sm);		// 2
    HostState hs(&sm);		// 3
    GameState gs(&sm);		// 4
    EndState es(&sm);		// 5
	OptionsState os(&sm);	// 6

    sm.addState(&s);
    sm.addState(&ms);
    sm.addState(&js);
    sm.addState(&hs);
    sm.addState(&gs);
    sm.addState(&es);
	sm.addState(&os);
    sm.push(0);
    sf::RenderWindow window(sf::VideoMode(Universal::window_width, Universal::window_height), "Jewel Heist", sf::Style::Close);
    sf::Clock clock;
    float lag = 0;
	bool focus = true;
    while(window.isOpen())
    {
        sf::Event e;
        std::string typed = "";
        while(window.pollEvent(e)) {
			if (e.type == sf::Event::Closed)
				window.close();
			else if (e.type == sf::Event::TextEntered && e.text.unicode < 128) {
				typed += static_cast<char>(e.text.unicode);
			}
			else if (e.type == sf::Event::GainedFocus)
				focus = true;
			else if (e.type == sf::Event::LostFocus)
				focus = false;
        }

		if (focus)
			sm.handleInput(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, typed, e);
		
        sm.update(Universal::spf);

        window.clear(sf::Color::Black);
        sm.draw(window);
        window.display();

        float timePassed = lag+clock.getElapsedTime().asSeconds();
        if(timePassed<Universal::spf) {
            float extra = Universal::spf-timePassed;
            sf::sleep(sf::seconds(extra));
            lag = 0;
        } else {
            lag = timePassed-Universal::spf;
        }
        clock.restart();
    }
    return 0;
}
