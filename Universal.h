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
	static constexpr float GAME_DURATION = 5.0f;


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

	static std::string SFKeyToString(unsigned int keycode) {
		std::string retValue;
		switch (keycode) {
		case sf::Keyboard::Escape: retValue = ("Escape"); break;
		case sf::Keyboard::LControl: retValue = ("LControl"); break;
		case sf::Keyboard::LShift: retValue = ("LShift"); break;
		case sf::Keyboard::LAlt: retValue = ("LAlt"); break;
		case sf::Keyboard::LSystem: retValue = ("LSystem"); break;
		case sf::Keyboard::RControl: retValue = ("RControl"); break;
		case sf::Keyboard::RShift: retValue = ("RShift"); break;
		case sf::Keyboard::RAlt: retValue = ("RAlt"); break;
		case sf::Keyboard::RSystem: retValue = ("RSystem"); break;
		case sf::Keyboard::Menu: retValue = ("Menu"); break;
		case sf::Keyboard::LBracket: retValue = ("LBracket"); break;
		case sf::Keyboard::RBracket: retValue = ("RBracket"); break;
		case sf::Keyboard::SemiColon: retValue = (";"); break;
		case sf::Keyboard::Comma: retValue = (","); break;
		case sf::Keyboard::Period: retValue = ("."); break;
		case sf::Keyboard::Quote: retValue = ("\'"); break;
		case sf::Keyboard::Slash: retValue = ("/"); break;
		case sf::Keyboard::BackSlash: retValue = ("\\"); break;
		case sf::Keyboard::Tilde: retValue = ("~"); break;
		case sf::Keyboard::Equal: retValue = ("="); break;
		case sf::Keyboard::Dash: retValue = ("-"); break;
		case sf::Keyboard::Space: retValue = ("Space"); break;
		case sf::Keyboard::Return: retValue = ("Return"); break;
		case sf::Keyboard::BackSpace: retValue = ("Backspace"); break;
		case sf::Keyboard::Tab: retValue = ("Tab"); break;
		case sf::Keyboard::PageUp: retValue = ("Page Up"); break;
		case sf::Keyboard::PageDown: retValue = ("Page Down"); break;
		case sf::Keyboard::End: retValue = ("End"); break;
		case sf::Keyboard::Home: retValue = ("Home"); break;
		case sf::Keyboard::Insert: retValue = ("Insert"); break;
		case sf::Keyboard::Delete: retValue = ("Delete"); break;
		case sf::Keyboard::Add: retValue = ("+"); break;
		case sf::Keyboard::Subtract: retValue = ("-"); break;
		case sf::Keyboard::Multiply: retValue = ("*"); break;
		case sf::Keyboard::Divide: retValue = ("/"); break;
		case sf::Keyboard::Left: retValue = ("Left"); break;
		case sf::Keyboard::Right: retValue = ("Right"); break;
		case sf::Keyboard::Up: retValue = ("UP"); break;
		case sf::Keyboard::Down: retValue = ("Down"); break;
		case sf::Keyboard::Numpad0: retValue = ("NP 0"); break;
		case sf::Keyboard::Numpad1: retValue = ("NP 1"); break;
		case sf::Keyboard::Numpad2: retValue = ("NP 2"); break;
		case sf::Keyboard::Numpad3: retValue = ("NP 3"); break;
		case sf::Keyboard::Numpad4: retValue = ("NP 4"); break;
		case sf::Keyboard::Numpad5: retValue = ("NP 5"); break;
		case sf::Keyboard::Numpad6: retValue = ("NP 6"); break;
		case sf::Keyboard::Numpad7: retValue = ("NP 7"); break;
		case sf::Keyboard::Numpad8: retValue = ("NP 8"); break;
		case sf::Keyboard::Numpad9: retValue = ("NP 9"); break;
		case sf::Keyboard::F1: retValue = ("F1"); break;
		case sf::Keyboard::F2: retValue = ("F2"); break;
		case sf::Keyboard::F3: retValue = ("F3"); break;
		case sf::Keyboard::F4: retValue = ("F4"); break;
		case sf::Keyboard::F5: retValue = ("F5"); break;
		case sf::Keyboard::F6: retValue = ("F6"); break;
		case sf::Keyboard::F7: retValue = ("F7"); break;
		case sf::Keyboard::F8: retValue = ("F8"); break;
		case sf::Keyboard::F9: retValue = ("F9"); break;
		case sf::Keyboard::F10: retValue = ("F10"); break;
		case sf::Keyboard::F11: retValue = ("F11"); break;
		case sf::Keyboard::F12: retValue = ("F12"); break;
		case sf::Keyboard::F13: retValue = ("F13"); break;
		case sf::Keyboard::F14: retValue = ("F14"); break;
		case sf::Keyboard::F15: retValue = ("F15"); break;
		case sf::Keyboard::Pause: retValue = ("Paues"); break;

		default:
			retValue = ("%c", keycode);
		}

		std::cout << retValue << " from method" << std::endl;
		return retValue;
	}

};

#endif // UNIVERSAL_H_INCLUDED
