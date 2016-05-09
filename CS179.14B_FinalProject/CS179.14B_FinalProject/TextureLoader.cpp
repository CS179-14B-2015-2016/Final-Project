#include "TextureLoader.h"

sf::Texture* TextureLoader::getTexture(string n) {
	sf::Texture* temp = NULL;
	map<string, sf::Texture*>::const_iterator results = textures.find(n);
	if (results != textures.end()) {
		temp = results->second;
	}
	else {
		temp = new sf::Texture();
		if (!temp->loadFromFile(n)) {
			cout << "SFML could not find texture. Check Path.";
		}
			
		temp->setSmooth(true);
		textures.insert(pair<string, sf::Texture*>(n, temp));
		return temp;
	}
	return temp;
}
