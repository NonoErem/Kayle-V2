#pragma once  
#include <SFML/Graphics.hpp>  
#include <string>  
#include <functional>  
#include <iostream>  

class EditorPanel {
public:
    sf::RectangleShape panel;

    EditorPanel() : layer(0) {
        panel.setSize(sf::Vector2f(0.f, 0.f));
        panel.setPosition(0.f, 0.f);
        panel.setFillColor(sf::Color(30, 30, 30));
    }

    EditorPanel(float posX, float posY, float sizeX, float sizeY) : layer(0) {
        panel.setSize(sf::Vector2f(sizeX, sizeY));
        panel.setPosition(posX, posY);
        panel.setFillColor(sf::Color(30, 30, 30));
    }

    virtual void draw(sf::RenderWindow& window) {
        window.draw(panel);
    }

    void getLeftClickPosition(sf::Vector2i mousePos) {
        if (!select1) {
            select1 = true;
            select1_pos = mousePos;
			std::cout << "Select 1 : " << select1_pos.x << ", " << select1_pos.y << "\n";
        }
        else if (!select2) {
            select2 = true;
            select2_pos = mousePos;
            std::cout << "Select 2 : " << select2_pos.x << ", " << select2_pos.y << "\n";

        }
        else {
            select1 = false;
            select2 = false;
            std::cout << "Désélection\n";
        }
    }

    bool mouseInPanel(sf::Vector2i mousePos) {
        return panel.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

	int getLayer() const {
		return layer;
	}

private:
    bool select1 = false;
    bool select2 = false;
    sf::Vector2i select1_pos;
    sf::Vector2i select2_pos;
    int layer;

};

class TilesetPanel : public EditorPanel {
public:
    TilesetPanel(const std::string& tilesetPath, float posX, float posY, float sizeX, float sizeY)
        : EditorPanel(posX, posY, sizeX, sizeY) {
        if (!texture.loadFromFile(tilesetPath)) {
            std::cerr << "TilesetPanel : impossible de charger " << tilesetPath << "\n";
        }
        sprite.setTexture(texture);
        sprite.setPosition(posX, posY);
    }

    void draw(sf::RenderWindow& window) override {
        EditorPanel::draw(window);
        window.draw(sprite);
    }

private:
    sf::Texture texture;
    sf::Sprite sprite;
};
