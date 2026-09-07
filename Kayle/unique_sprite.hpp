#pragma once
#include <SFML/Graphics.hpp>

class Unique_sprite {
public:
	Unique_sprite() /* : id(next_id++) */{}
	sf::Sprite sprite;
	int id=-1;
	std::string filePath;
	int defaultLayer = 0;
private:
	//inline static int next_id = 1;
};
