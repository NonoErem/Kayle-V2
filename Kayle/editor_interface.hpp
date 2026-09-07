#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "globalvariables.hpp"
#include "map.hpp"
#include <iostream>
#include "sprites_manager.hpp"
#include <algorithm>

class OutlineBox {
public:
	OutlineBox(int posX, int posY, int sizeX, int sizeY);
	sf::RectangleShape shape;
};

class Items {
public:
	Items();
	sf::RectangleShape shape;
	sf::Text label;
	Unique_sprite uniqueSprite;
	bool isSelected = false;
};

class Button : public Items {
public:
	Button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize);

};

class Sprite_button : public Items {
public:
	Sprite_button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize);
};

class Editor_interface {
public:
	Editor_interface(Sprites_manager& spritesManager);
	sf::RectangleShape shape;
	void displayPanel(sf::RenderWindow& window) const;
	void panel(Sprites_manager& spritesManager);
	void updatePanel(Sprites_manager& spritesManager);

	std::vector<Items> panelItems;
	sf::Font font;
	int cursorOffset = 0;
	int layer = 0;
	OutlineBox outlineBox;
};




/*
class Interface_item {
public:
	virtual ~Interface_item() = default;
	sf::RectangleShape shape;
	sf::Text label;
	std::function<void()> objectAction;
	virtual void display(sf::RenderWindow& window) const {
		window.draw(shape);
		window.draw(label);
	}
private:

};

class Interface_button : public Interface_item {
public:
	Interface_button(sf::RenderWindow& window, Map& map, std::string stringLabel, sf::Font& font, std::function<void()> action, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize);
};

class Interface_panel : public Interface_item {
public:
	Interface_panel(sf::RenderWindow& window, Map& map, Sprites_manager& spritesManager, sf::Font& font, int posX, int posY, int sizeX, int sizeY);
	std::vector<std::unique_ptr<Interface_item>> panelItems;
};

class Sprite_selector : public Interface_item {
public:
	Sprites_manager& spritesManagerHere;

	Sprite_selector(sf::RenderWindow& window, Sprites_manager& spritesManager, sf::Font& font, int posX, int posY, int sizeX, int sizeY);
	void update();
	void display(sf::RenderWindow& window) const;

	std::vector<sf::Sprite> spriteButtons;

	int cursorOffset = 0;
	int selectionPosition = 0;
private:
	int pos_X;
	int pos_Y;
	int size_X;
	int size_Y;
};

class Editor_interface {
public:
	Editor_interface(sf::RenderWindow& window, Map& map, Sprites_manager& spritesManager);
	void displayInterface(sf::RenderWindow& window);
	std::vector<std::unique_ptr<Interface_panel>> interfacePanels;
	Map& theMap;
	Unique_sprite selectedSprite;
private:
	sf::Font font;
};
*/