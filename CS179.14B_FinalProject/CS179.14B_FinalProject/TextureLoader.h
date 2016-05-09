#pragma once

#include <map>
#include <iostream>

#include <SFML/Graphics.hpp>

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
