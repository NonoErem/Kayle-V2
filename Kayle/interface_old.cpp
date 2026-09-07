#include "interface_old.hpp"
#include "config.hpp"
#include <iostream>



HP_Bar::HP_Bar()
{
	sprite.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT * 0.5f);

}


Interface_old::Interface_old(Player* player)
{
	if (!hp_bar.texture[0].loadFromFile("Textures/interface/hp_bar_0%.png")) {
		std::cout << "Erreur de chargement de la texture de la barre de HP !" << std::endl;
	}
	if (!hp_bar.texture[1].loadFromFile("Textures/interface/hp_bar_25%.png")) {
		std::cout << "Erreur de chargement de la texture de la barre de HP !" << std::endl;
	}
	if (!hp_bar.texture[2].loadFromFile("Textures/interface/hp_bar_50%.png")) {
		std::cout << "Erreur de chargement de la texture de la barre de HP !" << std::endl;
	}
	if (!hp_bar.texture[3].loadFromFile("Textures/interface/hp_bar_75%.png")) {
		std::cout << "Erreur de chargement de la texture de la barre de HP !" << std::endl;
	}
	if (!hp_bar.texture[4].loadFromFile("Textures/interface/hp_bar_100%.png")) {
		std::cout << "Erreur de chargement de la texture de la barre de HP !" << std::endl;
	}

	
	if (player->HP >= 0.9 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[4]);

	}
	else if (player->HP >= 0.75 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[3]);
	}
	else if (player->HP >= 0.5 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[2]);
	}
	else if (player->HP >= 0.25 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[1]);	}
	else {
		hp_bar.sprite.setTexture(hp_bar.texture[0]);
	}

	std::cout << hp_bar.sprite.getGlobalBounds().width << "\n";

	float position_x = SCREEN_WIDTH / 2.f - hp_bar.sprite.getGlobalBounds().width / 2.f;
	float position_y = SCREEN_HEIGHT * 0.8f;
	hp_bar.sprite.setPosition(position_x, position_y);



}

void Interface_old::draw(sf::RenderWindow& window) {
	window.draw(hp_bar.sprite);

}

void Interface_old::update(Player* player) {


	if (player->HP >= 0.9 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[4]);
	}
	else if (player->HP >= 0.75 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[3]);
	}
	else if (player->HP >= 0.5 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[2]);
	}
	else if (player->HP >= 0.25 * player->maxHP) {
		hp_bar.sprite.setTexture(hp_bar.texture[1]);
	}
	else {
		hp_bar.sprite.setTexture(hp_bar.texture[0]);
	}
}