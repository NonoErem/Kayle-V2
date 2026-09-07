#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "windows_things.hpp"
#include "unique_sprite.hpp"
#include "globalvariables.hpp"



class Sprites_manager {
public:
	Sprites_manager();
	std::vector<Unique_sprite> allUniqueSprites;
	const Unique_sprite& getSprite(int id) const;
	int askDefaultLayer(sf::RenderWindow& window);
	void newSprite(sf::RenderWindow& window, std::string selectedFile, int defaultLayer);
	void saveSpriteSet(const std::string& filePath);
	void loadSpriteSet(sf::RenderWindow& window, const std::string& filePath);

	std::vector<std::unique_ptr<sf::Texture>> m_textures;
};