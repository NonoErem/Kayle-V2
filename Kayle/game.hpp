#pragma once
#include <SFML/Graphics.hpp>
#include "sprites_manager.hpp"
#include "map.hpp"
#include "globalvariables.hpp"
#include "inputHandler.hpp"
#include "player.hpp"

class Game
{
public:
	Game(sf::RenderWindow& window); //Make sure to initialize the editor with a map and interface
	void run(sf::RenderWindow& window);
	private:
};