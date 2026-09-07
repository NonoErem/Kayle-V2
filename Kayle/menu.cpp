#include "menu.hpp"
#include "globalvariables.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>


Menu::Menu(const std::vector<MenuItem>& menuItems, const std::string& bgDirectory) {
    if (!font.loadFromFile("ARIAL.TTF")) {
		std::cerr << "Erreur lors du chargement de la police ARIAL.TTF" << std::endl;
    }

    if (!texture.loadFromFile(bgDirectory)) {
		std::cerr << "Erreur lors du chargement du BG du menu principal" << std::endl;
    }
	sprite.setTexture(texture);
    sprite.setScale(
        static_cast<float>(SCREEN_WIDTH) / texture.getSize().x,
        static_cast<float>(SCREEN_HEIGHT) / texture.getSize().y
    );

    items = menuItems;

    for (size_t i = 0; i < items.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(48);
        text.setFillColor(sf::Color::White);
        
        text.setString(items[i].label);
        text.setStyle(sf::Text::Regular);
        float textWidth = text.getLocalBounds().width;
        text.setPosition(SCREEN_WIDTH / 2.f - textWidth / 2.f, SCREEN_HEIGHT / 2.f - (items.size() - i) * (characterSize+padding));

        menuTexts.push_back(text);
    }
}

void Menu::selectCurrentOption() {
    int index = cursorPosition - 1;
    if (index >= 0 && index < static_cast<int>(items.size())) {
        if (items[index].action) {
            items[index].action(); // Exécution de l'action
        }
    }
}

void Menu::displayMenu(sf::RenderWindow& window) {
    
    //Display cursor (by changing elements color and size)
    for (size_t i = 0; i < items.size(); ++i) {
		if (i == cursorPosition - 1) {
			menuTexts[i].setFillColor(sf::Color::Yellow);
            menuTexts[i].setStyle(sf::Text::Bold);
		}
		else {
			menuTexts[i].setFillColor(sf::Color::White);
            menuTexts[i].setStyle(sf::Text::Regular);
		}
    }

    //Display background
    window.draw(sprite);
    //Display Text
    for (const sf::Text& text : menuTexts) {
        window.draw(text);
    }
}



/*void Menu::displayRebindKeyMessage(std::string actionName) {
    print at cursorPosition;

*/