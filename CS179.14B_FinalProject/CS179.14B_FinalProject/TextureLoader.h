#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

using namespace std;
class TextureLoader {
protected:
	map<std::string, sf::Texture*> textures;
public:
	TextureLoader() {}
	~TextureLoader() {
		map<std::string, sf::Texture*>::iterator itr = textures.begin();
		delete itr->second;
		textures.erase(itr);
	}

	sf::Texture* getTexture(string n);
};

extern TextureLoader tl;