#ifndef UNIVERSAL_H_INCLUDED
#define UNIVERSAL_H_INCLUDED

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

struct Universal {
	static constexpr int window_width = 1024;
	static constexpr int window_height = 800;
	static constexpr float fps = 30.0;
	static constexpr float spf = 1.0 / fps;
	static constexpr float GAME_DURATION = 90.0f;
	static constexpr int ENTITY_COUNT = 15;

	static sf::Font getFont() {
		static sf::Font font;
		static bool loaded = false;
		if (!loaded)
		{
			font.loadFromFile("assets/fonts/black_jack.ttf");
			loaded = true;
		}
		return font;
	}

	static float clamp(float var, float mini, float maxi)
	{
		if (var < mini) return mini;
		else if (var > maxi) return maxi;
		return var;
	}

	static void appendTextInput(std::string& base, const std::string& append)
	{
		for (int i = 0; i<append.length(); i++)
		{
			if (append[i] != '\n' && (int)append[i] != 13)
			{
				if (append[i] != '\b')
					base += append[i];
				else if (base.length()>0)
					base = base.substr(0, base.length() - 1);
			}
		}
	}

};

#endif // UNIVERSAL_H_INCLUDED
