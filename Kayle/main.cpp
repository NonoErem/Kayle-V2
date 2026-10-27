#include <SFML/Graphics.hpp>
#include <iostream>
#include "globalvariables.hpp"
#include "menu.hpp"
#include "inputHandler.hpp"
#include "map.hpp"
#include "editor.hpp"
#include "game.hpp"

void launchGame(sf::RenderWindow& window, InputHandler& inputHandler) {
	std::cout << "Launching Game..." << std::endl;
	Game game(window);
}
void launchEditor(sf::RenderWindow& window) {
	std::cout << "Launching Editor..." << std::endl;
	Editor editor(window);
}



void mainMenu() {
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kayle");
	InputHandler inputHandler;

	
	Menu menu(
		{ 
			{ "Jouer", [&]() { BaseGameState = PLAYING; } },
			{ "Editeur", [&]() { BaseGameState = EDITING; } },
			{ "Options", [&]() { BaseGameState = MAINSETTINGS; } },
			{ "Quitter", [&]() { BaseGameState = QUITTING; } }
		},
		"Textures/mainMenu/BG_placeholder.png");

	Menu settingsMenu(
		{
			{ "Changer 'haut'", [&]() { inputHandler.letPlayerAddKey(Action::Up, window); } },
			{ "Changer 'bas'", [&]() { inputHandler.letPlayerAddKey(Action::Down, window); } },
			{ "Changer 'sélectionner'", [&]() { inputHandler.letPlayerAddKey(Action::Select, window); } },
			{ "Retour", [&]() { BaseGameState = MAINMENU; } }
		},
		"Textures/mainMenu/BG_placeholder.png");

	sf::Clock clock;
	

	while (window.isOpen()) {
		window.clear(sf::Color::Black);

		deltaTime = clock.restart().asSeconds();

		switch (BaseGameState) {
		case MAINMENU:
			menu.displayMenu(window);
			inputHandler.input(window, menu);
			break;
		case PLAYING:
			launchGame(window, inputHandler);
			std::cerr << "Back here" << std::endl;
			break;
		case EDITING:
			launchEditor(window);
			break;
		case QUITTING:
			window.close();
			break;
		case MAINSETTINGS:
			settingsMenu.displayMenu(window);
			inputHandler.input(window, settingsMenu);
			break;
		}
		window.display();

	}
}

int main() {
	mainMenu();
}