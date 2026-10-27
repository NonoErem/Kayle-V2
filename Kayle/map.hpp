#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "sprites_manager.hpp"
#include "globalvariables.hpp"
#include "entity.hpp"
#include "effect.hpp"
#include <utility>

class Map
{
public:
	Map(Sprites_manager& spritesManager);
	void loadFromFile(const std::string& filePath);
	void saveToFile(const std::string& filePath);

    void displayMap(sf::RenderWindow& window, sf::Sprite playerSprite = sf::Sprite());
	void drawLayer(sf::RenderWindow& window, int layer);
	void setTile(Unique_tile& sprite, sf::Vector2f mousePos, int layer, int brushSize);
	void setEntity(Unique_entity& entity, sf::Vector2f mousePos, int layer);
	void resetTile(sf::Vector2f mousePos, int brushSize);

	void removeActor(sf::Vector2f mousePos, int brushSize);

	void updateEntities(float dt);
	bool checkCollision(const sf::FloatRect& bounds) const;
	void handlePlayerAttack(const sf::FloatRect& attackBounds, int damage);

	void setEffect(int effectId, int posX, int posY, float scale, int layer);
	void updateEffects(float deltaTime);
private:
	std::vector<std::vector<std::vector<Unique_tile>>> tiles;
	std::vector<Entity> entities;
	std::vector<Effect> effects;
	Sprites_manager& spritesManager;
	int MAP_WIDTH = 1;
	int MAP_HEIGHT = 1;
};
