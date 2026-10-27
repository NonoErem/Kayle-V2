#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "windows_things.hpp"
#include "unique_tile.hpp"
#include "unique_entity.hpp"
#include "unique_effect.hpp"
#include "globalvariables.hpp"



class Sprites_manager {
public:
	Sprites_manager();
	std::vector<Unique_tile> allUniqueTiles;
	std::vector<Unique_entity> allUniqueEntities;
	std::vector<Unique_effect> allUniqueEffects;
	const Unique_tile& getUniqueTile(int id) const;
	const Unique_entity& getUniqueEntity(int id) const;
	const Unique_effect& getUniqueEffect(int id) const;
	int askDefaultLayer(sf::RenderWindow& window);
	void newTile(sf::RenderWindow& window, std::string selectedFile, int defaultLayer, bool blockEntity, bool blockLight);
	void saveSpriteSet(const std::string& filePath);
	void loadSpriteSet(sf::RenderWindow& window, const std::string& filePath);



	void newEntity(sf::RenderWindow& window, std::string selectedFile, int defaultLayer);
	void saveEntitySet(const std::string& filename);
	void loadEntitySet(sf::RenderWindow& window, const std::string& filename);

	void newEffect(sf::RenderWindow& window, std::string selectedFile, int defaultLayer, int frameWidth = 0, int frameHeight = 0, int frameCount = 1, float frameDuration = 0.05f, bool looping = false);

	void loadEffectSet(sf::RenderWindow& window, const std::string& filePath);

	std::vector<std::unique_ptr<sf::Texture>> m_textures;
};