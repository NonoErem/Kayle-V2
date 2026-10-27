#include "editor_interface.hpp"
#include <SFML/Graphics.hpp>
#include "windows_things.hpp"
#include <cmath>

Editor_interface::Editor_interface(Sprites_manager& spritesManager) : outlineBox(0, 0, TILE_SIZE, TILE_SIZE) {
    if (!font.loadFromFile("ARIAL.TTF")) {
        std::cerr << "Erreur lors du chargement de la police ARIAL.TTF" << std::endl;
    }

    // Configuration de la forme principale du panneau
    int posX = SCREEN_WIDTH * 0.8; // 1 - 0.2
    int posY = SCREEN_HEIGHT * 0.1; // 1 - 0.9
    int sizeX = SCREEN_WIDTH * 0.2;
    int sizeY = SCREEN_HEIGHT * 0.8;

    shape.setSize(sf::Vector2f(sizeX, sizeY));
    shape.setPosition(posX, posY);
    shape.setFillColor(sf::Color::White); // Optionnel : un fond semi-transparent pour plus de lisibilité

    // Initialisation unique des éléments
    initPanel(spritesManager);
}

void Editor_interface::initPanel(Sprites_manager& spritesManager) {
    panelItems.clear();

    int posX = shape.getPosition().x;
    int posY = shape.getPosition().y;
    int sizeX = shape.getSize().x;
    int sizeY = shape.getSize().y;

    int buttonSizeX = 110;
    int buttonSizeY = 40;
    float characterSize = 0.8f;

    // --- Ligne 1 : Actions principales ---
    int posY_line1 = posY + sizeY * 0.01;
    panelItems.push_back(std::make_unique<Button>("Add", font, posX + sizeX * 0.05, posY_line1, buttonSizeX, buttonSizeY, characterSize));
    panelItems.push_back(std::make_unique<Button>("Save", font, posX + sizeX * 0.35, posY_line1, buttonSizeX, buttonSizeY, characterSize));
    panelItems.push_back(std::make_unique<Button>("Load", font, posX + sizeX * 0.65, posY_line1, buttonSizeX, buttonSizeY, characterSize));

    // --- Ligne 2 : Outils d'édition ---
    int posY_line2 = posY + sizeY * 0.11;
    panelItems.push_back(std::make_unique<Button>("Paint", font, posX + sizeX * 0.05, posY_line2, buttonSizeX, buttonSizeY, characterSize));

    int smallBtnX = 70;
    float smallCharSize = 0.4f;
    panelItems.push_back(std::make_unique<Button>("Erase\nall", font, posX + sizeX * 0.35, posY_line2, smallBtnX, buttonSizeY, smallCharSize));
    panelItems.push_back(std::make_unique<Button>("Remove\ntiles", font, posX + sizeX * 0.55, posY_line2, smallBtnX, buttonSizeY, smallCharSize));
    panelItems.push_back(std::make_unique<Button>("Remove\nactors", font, posX + sizeX * 0.75, posY_line2, smallBtnX, buttonSizeY, smallCharSize));

    // --- Ligne 3 : Sélection du Panneau (Tiles / Entities) ---
    int posY_panelSelect = posY + sizeY * 0.21;
    int panelBtnWidth = sizeX * 0.42;

    auto tilesBtn = std::make_unique<Button>("Panel Tiles", font, posX + sizeX * 0.05, posY_panelSelect, panelBtnWidth, buttonSizeY, smallCharSize);
    // On met en valeur le bouton actif (Cyan si actif, Bleu par défaut)
    if (currentPanel == EditorPanel::Tiles) {
        tilesBtn->shape.setFillColor(sf::Color::Cyan);
        tilesBtn->label.setFillColor(sf::Color::Black);
    }
    panelItems.push_back(std::move(tilesBtn));

    auto entitiesBtn = std::make_unique<Button>("Panel Entities", font, posX + sizeX * 0.53, posY_panelSelect, panelBtnWidth, buttonSizeY, smallCharSize);
    if (currentPanel == EditorPanel::Entities) {
        entitiesBtn->shape.setFillColor(sf::Color::Cyan);
        entitiesBtn->label.setFillColor(sf::Color::Black);
    }
    panelItems.push_back(std::move(entitiesBtn));

    // --- Sélecteur de Sprites (décalé à 0.31 pour laisser la place) ---
    int posY_spriteSelector = posY + sizeY * 0.31;
    int spritesByPage = 5;

    if (currentPanel == EditorPanel::Tiles) {
        int endForLoop = std::min(spritesByPage + cursorOffset, static_cast<int>(spritesManager.allUniqueTiles.size()));

        for (int i = cursorOffset; i < endForLoop; i++) {
            std::string label = std::to_string(spritesManager.allUniqueTiles[i].defaultLayer);

            auto spriteButton = std::make_unique<Sprite_button>(
                label, font, posX + sizeX * 0.05,
                posY_spriteSelector + sizeY * 0.6 * 0.2f * (i - cursorOffset),
                TILE_SIZE, TILE_SIZE, smallCharSize
            );

            spriteButton->uniqueSprite = &spritesManager.allUniqueTiles[i];
            spriteButton->uniqueSprite->sprite.setPosition(
                std::floor(posX + sizeX * 0.05),
                std::floor(posY_spriteSelector + sizeY * 0.6 * 0.2f * (i - cursorOffset))
            );

            spriteButton->blockEntityShape.setFillColor(spriteButton->uniqueSprite->blockEntity ? sf::Color::Green : sf::Color::Red);
            spriteButton->blockLightShape.setFillColor(spriteButton->uniqueSprite->blockLight ? sf::Color::Green : sf::Color::Red);

            panelItems.push_back(std::move(spriteButton));
        }
    }
    else {
        int endForLoop = std::min(spritesByPage + cursorOffset, static_cast<int>(spritesManager.allUniqueEntities.size()));

        for (int i = cursorOffset; i < endForLoop; i++) {
            std::string label = std::to_string(spritesManager.allUniqueEntities[i].defaultLayer);

            auto spriteButton = std::make_unique<Sprite_button>(
                label, font, posX + sizeX * 0.05,
                posY_spriteSelector + sizeY * 0.6 * 0.2f * (i - cursorOffset),
                TILE_SIZE, TILE_SIZE, smallCharSize
            );

            spriteButton->uniqueEntity = &spritesManager.allUniqueEntities[i];
            spriteButton->uniqueEntity->sprite.setPosition(
                std::floor(posX + sizeX * 0.05),
                std::floor(posY_spriteSelector + sizeY * 0.6 * 0.2f * (i - cursorOffset))
            );

            spriteButton->blockEntityShape.setFillColor(spriteButton->uniqueEntity->blockEntity ? sf::Color::Green : sf::Color::Red);
            spriteButton->blockLightShape.setFillColor(spriteButton->uniqueEntity->blockLight ? sf::Color::Green : sf::Color::Red);

            panelItems.push_back(std::move(spriteButton));
        }
    }
    // --- Ligne des calques (Layers) ---
    int posY_layerLine = posY + sizeY * 0.81;
    int layerBtnSizeX = 40;
    panelItems.push_back(std::make_unique<Button>("Deflt\nLayer", font, posX + sizeX * 0.05, posY_layerLine, layerBtnSizeX, buttonSizeY, smallCharSize));
    panelItems.push_back(std::make_unique<Button>(" Lay\n er 0", font, posX + sizeX * 0.20, posY_layerLine, layerBtnSizeX, buttonSizeY, smallCharSize));
    panelItems.push_back(std::make_unique<Button>(" Lay\n er 1", font, posX + sizeX * 0.35, posY_layerLine, layerBtnSizeX, buttonSizeY, smallCharSize));
    panelItems.push_back(std::make_unique<Button>(" Lay\n er 2", font, posX + sizeX * 0.50, posY_layerLine, layerBtnSizeX, buttonSizeY, smallCharSize));
    panelItems.push_back(std::make_unique<Button>(" Lay\n er 3", font, posX + sizeX * 0.65, posY_layerLine, layerBtnSizeX, buttonSizeY, smallCharSize));
    panelItems.push_back(std::make_unique<Button>(" Lay\n er 4", font, posX + sizeX * 0.80, posY_layerLine, layerBtnSizeX, buttonSizeY, smallCharSize));

    // --- Ligne de la taille du pinceau (Brush) ---
    int posY_brushLine = posY + sizeY * 0.91;
    float brushCharacterSize = 0.9f;
    int brushSquareSize = buttonSizeY;

    panelItems.push_back(std::make_unique<Button>(" -", font, posX + sizeX * 0.05, posY_brushLine, brushSquareSize, buttonSizeY, brushCharacterSize));
    panelItems.push_back(std::make_unique<Button>(std::to_string(brushSize), font, posX + sizeX * 0.20, posY_brushLine, brushSquareSize, buttonSizeY, brushCharacterSize));
    panelItems.push_back(std::make_unique<Button>(" +", font, posX + sizeX * 0.35, posY_brushLine, brushSquareSize, buttonSizeY, brushCharacterSize));
}

// Appelé uniquement lorsque l'utilisateur change de page ou modifie une valeur (ex: brushSize)
void Editor_interface::updatePanel(Sprites_manager& spritesManager) {
    initPanel(spritesManager);
}

void Editor_interface::displayPanel(sf::RenderWindow& window) const {
    window.draw(shape);
    for (const auto& itemPtr : panelItems) {
        const auto& item = *itemPtr; // Déférencement du unique_ptr
        window.draw(item.shape);
        if (item.uniqueSprite != nullptr)
            window.draw(item.uniqueSprite->sprite);
        if (item.uniqueEntity != nullptr)
            window.draw(item.uniqueEntity->sprite);
        window.draw(item.label);
        window.draw(item.blockEntityShape);
        window.draw(item.blockEntityLabel);
        window.draw(item.blockLightShape);
        window.draw(item.blockLightLabel);
    }
    window.draw(outlineBox.shape);
}
Items::Items() {
    shape.setFillColor(sf::Color::Blue);
}

Button::Button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize) {
    shape.setSize(sf::Vector2f(buttonSizeX, buttonSizeY));
    shape.setPosition(posX, posY);
    shape.setFillColor(sf::Color::Blue);

    label.setFont(font);
    label.setCharacterSize(buttonSizeY * characterSize);
    label.setFillColor(sf::Color::Red);
    label.setString(stringLabel);
    label.setStyle(sf::Text::Regular);
    label.setPosition(posX, posY);
}

Sprite_button::Sprite_button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize) {
    shape.setSize(sf::Vector2f(buttonSizeX, buttonSizeY));
    shape.setPosition(posX, posY);
    shape.setFillColor(sf::Color::Blue);

    label.setFont(font);
    label.setCharacterSize(buttonSizeY * characterSize);
    label.setFillColor(sf::Color::Red);
    label.setString(stringLabel);
    label.setStyle(sf::Text::Regular);
    label.setPosition(posX + TILE_SIZE, posY);

    float squareSize = buttonSizeY / 2.f;

    blockEntityShape.setSize(sf::Vector2f(buttonSizeX / 2.f, buttonSizeY / 2.f));
    blockEntityShape.setPosition(posX + buttonSizeX * 2, posY);
    blockEntityShape.setFillColor(sf::Color::Blue);

    blockEntityLabel.setFont(font);
    blockEntityLabel.setCharacterSize(buttonSizeY * characterSize);
    blockEntityLabel.setFillColor(sf::Color::Red);
    blockEntityLabel.setString("blockEntity");
    blockEntityLabel.setStyle(sf::Text::Regular);
    blockEntityLabel.setPosition(posX + buttonSizeX * 2.f + squareSize + 5.f, posY);

    blockLightShape.setSize(sf::Vector2f(buttonSizeX / 2.f, buttonSizeY / 2.f));
    blockLightShape.setPosition(posX + buttonSizeX * 2.f, posY + buttonSizeY / 2.f);
    blockLightShape.setFillColor(sf::Color::Blue);

    blockLightLabel.setFont(font);
    blockLightLabel.setCharacterSize(buttonSizeY * characterSize);
    blockLightLabel.setFillColor(sf::Color::Red);
    blockLightLabel.setString("blockLight");
    blockLightLabel.setStyle(sf::Text::Regular);
    blockLightLabel.setPosition(posX + buttonSizeX * 2.f + squareSize + 5.f, posY + buttonSizeY / 2.f);
}

OutlineBox::OutlineBox(int posX, int posY, int sizeX, int sizeY) {
    shape.setPosition(posX - 2, posY - 2);
    shape.setSize(sf::Vector2f(sizeX, sizeY));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Red);
    shape.setOutlineThickness(2.f);
}