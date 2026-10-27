#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "globalvariables.hpp"
#include "map.hpp"
#include <iostream>
#include "sprites_manager.hpp"
#include <algorithm>
#include <memory>

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
    Unique_tile* uniqueSprite = nullptr;
	Unique_entity* uniqueEntity = nullptr;
    bool isSelected = false;
    sf::RectangleShape blockEntityShape;
    sf::Text blockEntityLabel;
    sf::RectangleShape blockLightShape;
    sf::Text blockLightLabel;
};

class Button : public Items {
public:
    Button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize);
};

class Sprite_button : public Items {
public:
    Sprite_button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize);
};


enum class EditorPanel {
    Tiles,
    Entities
};
class Editor_interface {
public:
    Editor_interface(Sprites_manager& spritesManager);

    sf::RectangleShape shape;
    void displayPanel(sf::RenderWindow& window) const;

    // Appelée une seule fois à la construction (ou si la fenêtre est redimensionnée)
    void initPanel(Sprites_manager& spritesManager);

    // Appelée uniquement lorsque l'état change (changement de page, de brush, etc.)
    void updatePanel(Sprites_manager& spritesManager);

    std::vector<std::unique_ptr<Items>> panelItems;
    sf::Font font;
    int cursorOffset = 0;
    int layer = 0;
    OutlineBox outlineBox;
    int brushSize = 3;

    EditorPanel currentPanel = EditorPanel::Tiles;
};