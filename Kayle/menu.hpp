#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include <functional>


struct MenuItem {
	std::string label;
	std::function<void()> action;
};

class Menu {
public:
	Menu(const std::vector<MenuItem>& menuItems, const std::string& bgDirectory);
	void displayMenu(sf::RenderWindow& window);
	void selectCurrentOption();
	std::vector<MenuItem> items;
	std::vector<sf::Text> menuTexts;
	int cursorPosition = 1;
private:
	sf::Font font;
	int characterSize = 48;
	int padding = 10;
	sf::Texture texture;
	sf::Sprite sprite;
};