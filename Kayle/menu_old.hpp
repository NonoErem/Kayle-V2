#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class GameState {
    MENU,
    PLAYING,
    EDITOR,
    EXIT
};

class Menu {
public:
    Menu(float width, float height);
    void draw(sf::RenderWindow& window);
    void handleInput(sf::Event event, GameState& state);

private:
    std::vector<sf::Text> options;
    int selectedIndex = 0;
    sf::Font font;

    void moveUp();
    void moveDown();
};
