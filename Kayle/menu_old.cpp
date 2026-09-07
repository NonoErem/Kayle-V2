#include "menu_old.hpp"

Menu::Menu(float width, float height) {
    font.loadFromFile("ARIAL.TTF");

    std::vector<std::string> labels = { "Jouer", "Editeur de carte", "Quitter" };
    for (size_t i = 0; i < labels.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(48);
        text.setFillColor(i == 0 ? sf::Color::Yellow : sf::Color::White);
        text.setPosition(width / 2 - 150, height / 3 + i * 60);
        options.push_back(text);
    }
}

void Menu::draw(sf::RenderWindow& window) {
    for (const auto& option : options) {
        window.draw(option);
    }
}

void Menu::handleInput(sf::Event event, GameState& state) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            moveUp();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            moveDown();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            switch (selectedIndex) {
            case 0: state = GameState::PLAYING; break;
            case 1: state = GameState::EDITOR; break;
            case 2: state = GameState::EXIT; break;
            }
        }
    }
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}

void Menu::moveDown() {
    if (selectedIndex < options.size() - 1) {
        options[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        options[selectedIndex].setFillColor(sf::Color::Yellow);
    }
}
