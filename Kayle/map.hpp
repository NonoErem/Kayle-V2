#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "sprites_manager.hpp"
#include "globalvariables.hpp"

class Map
{
public:
	Map(Sprites_manager& spritesManager);
	void loadFromFile(const std::string& filePath);
	void saveToFile(const std::string& filePath);

	void displayMap(sf::RenderWindow& window);
	void drawLayer(sf::RenderWindow& window, int layer);
	void setTile(Unique_sprite& sprite, sf::Vector2f mousePos, int layer);
	void resetTile(sf::Vector2f mousePos);
private:
	std::vector<std::vector<std::vector<Unique_sprite>>> tiles;
	Sprites_manager& spritesManager;
	int MAP_WIDTH = 1;
	int MAP_HEIGHT = 1;
};
