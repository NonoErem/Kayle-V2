#pragma once
#include <SFML/Graphics.hpp>

class Unique_tile {
public:
	Unique_tile() /* : id(next_id++) */ {};
	sf::Sprite sprite;
	int id = -1;
	std::string filePath;
	int defaultLayer = 0;

	bool blockEntity = false;
	bool blockLight = false;

private:
	//inline static int next_id = 1;
};
