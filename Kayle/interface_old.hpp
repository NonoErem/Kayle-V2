#pragma once
#include <SFML/Graphics.hpp>
#include "player.hpp"


class HP_Bar {
public:
	HP_Bar();
	sf::Texture texture[5]; // Texture de l'acteur
	sf::Sprite sprite;
};

class Interface_old {
public:
	Interface_old(Player* player);
	void draw(sf::RenderWindow& window);

	void update(Player* player);

private:
	HP_Bar hp_bar;
};


