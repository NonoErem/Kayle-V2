
#include "inputHandler.hpp"
#include "globalvariables.hpp"
#include "menu.hpp"

#include <iostream>
#include <SFML/Graphics.hpp>



InputHandler::InputHandler() : currentMenu(currentMenu)
{
    keyBindings[Action::Up]     = { sf::Keyboard::Up, sf::Keyboard::Z };
    keyBindings[Action::Down]   = { sf::Keyboard::Down, sf::Keyboard::S };
    keyBindings[Action::Select] = { sf::Keyboard::Enter, sf::Keyboard::E };
}

void InputHandler::bindKey(Action action, sf::Keyboard::Key key) {
    keyBindings[action].push_back(key);
}

void InputHandler::setKeys(Action action, const std::vector<sf::Keyboard::Key>& keys) {
    keyBindings[action] = keys;
}

bool InputHandler::isActionPressed(Action action, sf::Keyboard::Key key) const {
    auto it = keyBindings.find(action);
    if (it != keyBindings.end()) {
        const auto& keys = it->second;
        return std::find(keys.begin(), keys.end(), key) != keys.end();
    }
    return false;
}



void InputHandler::input(sf::RenderWindow& window, Menu& currentMenu) {
    cooldownUpdate();

    sf::Event event;

    //Loop to manage every events
    while (window.pollEvent(event)) {

        if (event.type == sf::Event::KeyPressed) {
            if (cooldownLeft <= 0) {
                if (isActionPressed(Action::Up, event.key.code)) {
                    moveUp(currentMenu);
                }
                else if (isActionPressed(Action::Down, event.key.code)) {
                    moveDown(currentMenu);
                }
                else if (isActionPressed(Action::Select, event.key.code)) {
                    currentMenu.selectCurrentOption();
                }
            }
        }



        //Mouse handling

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (size_t i = 0; i < currentMenu.menuTexts.size(); ++i) {
            if (currentMenu.menuTexts[i].getGlobalBounds().contains(mousePos)) {
                currentMenu.cursorPosition = static_cast<int>(i) + 1;
            }
        }


        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                for (size_t i = 0; i < currentMenu.menuTexts.size(); ++i) {
                    if (currentMenu.menuTexts[i].getGlobalBounds().contains(mousePos)) {
                        currentMenu.selectCurrentOption();
                    }
                }
            }
        }
    }
}


void InputHandler::moveUp(Menu& currentMenu) {
    if (currentMenu.cursorPosition >= 2) {
        currentMenu.cursorPosition--;
        cooldownLeft = cooldown;
    }
}

void InputHandler::moveDown(Menu& currentMenu) {
    if (!currentMenu.items.empty() && currentMenu.cursorPosition < static_cast<int>(currentMenu.items.size())) {
        currentMenu.cursorPosition++;
        cooldownLeft = cooldown;
    }
}

void InputHandler::cooldownUpdate() {
    if (cooldownLeft > 0) {
        cooldownLeft -= deltaTime;
    }
}




//Move everything to menu.cpp
void InputHandler::letPlayerAddKey(Action action, sf::RenderWindow& window/*, Menu& currentMenu*/) {
	std::cout << "Press a key to bind to the action." << std::endl;
	// displayRebindKeyMessage(currentMenu.cursorPosition)
    bool keyPressed = false;
	while (!keyPressed) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				bindKey(action, event.key.code);
				keyPressed = true;
				std::cout << "Key bound successfully!" << std::endl;
				break;
			}
		}
	}
}







EditorInputHandler::EditorInputHandler(sf::RenderWindow& windowHere, Map& mapHere, EditorState editorStateHere, Unique_tile& selectedSpriteHere, Editor_interface& editorInterfaceHere, Sprites_manager& spritesManagerHere, sf::View& cameraHere) : window(windowHere), map(mapHere), editorState(editorStateHere), selectedSprite(selectedSpriteHere), editorInterface(editorInterfaceHere), spritesManager(spritesManagerHere), camera(cameraHere) {

}


void EditorInputHandler::editorInput() {
    //std::cerr << "in the input() function" << std::endl;
    cooldownUpdate();

    sf::Event event;

    // Loop to manage every event
    while (window.pollEvent(event)) {

        //Mouse handling

        sf::Vector2f mouseWorld = window.mapPixelToCoords(sf::Mouse::getPosition(window), camera);
        sf::Vector2f mouseInterface = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());

        if (event.type == sf::Event::Closed)
            window.close();
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            leftClick(mouseWorld, mouseInterface);
        }
		if (event.type == sf::Event::MouseWheelMoved) {
			if (event.mouseWheel.delta > 0) {
				std::cerr << "Mouse wheel up" << std::endl;
                mouseWheelUp(mouseWorld, mouseInterface);
			}
			else if (event.mouseWheel.delta < 0) {
				std::cerr << "Mouse wheel down" << std::endl;
                mouseWheelDown(mouseWorld, mouseInterface);

			}
		}
        if (event.type == sf::Event::KeyPressed) {
            sf::Vector2f cameraOffset(0.f, 0.f);

            float cameraSpeed = 500.f; // Adjust the speed as needed

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                cameraSpeed *= 10.f;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                cameraOffset.x -= cameraSpeed * deltaTime;
                camera.move(cameraOffset);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                cameraOffset.x += cameraSpeed * deltaTime;
                camera.move(cameraOffset);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                cameraOffset.y += cameraSpeed * deltaTime;
                camera.move(cameraOffset);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                cameraOffset.y -= cameraSpeed * deltaTime;
                camera.move(cameraOffset);
            }
        }
    }
}



void EditorInputHandler::leftClick(sf::Vector2f mouseWorld, sf::Vector2f mouseInterface)
{
    if (cooldownLeft > 0)
        return;
    else
        cooldownLeft = cooldown;

    bool isInPanel = false;
    if (editorInterface.shape.getGlobalBounds().contains(mouseInterface)) {
        isInPanel = true;
    }


    if (isInPanel) {
        for (size_t i = 0; i < editorInterface.panelItems.size(); ++i) {
            if (editorInterface.panelItems[i]->shape.getGlobalBounds().contains(mouseInterface)) {

                if (editorInterface.panelItems[i]->label.getString() == "Add") {
					std::cerr << "Add button clicked" << std::endl;
                    HWND hwnd = window.getSystemHandle();
                    std::string selectedFile = openFileDialog(hwnd);
                    int layer = 0;
                    layer = spritesManager.askDefaultLayer(window);
                    if (editorInterface.currentPanel == EditorPanel::Tiles)   spritesManager.newTile(window, selectedFile, layer, false, false);
                    else   spritesManager.newEntity(window, selectedFile, layer);
                }
                else if (editorInterface.panelItems[i]->label.getString() == "Save") {
					std::cerr << "Save button clicked" << std::endl;
					map.saveToFile("map.txt");
                    spritesManager.saveSpriteSet("tileSet.txt");
					spritesManager.saveEntitySet("entitySet.txt");
				}
                else if (editorInterface.panelItems[i]->label.getString() == "Load") {
					std::cerr << "Load button clicked" << std::endl;
                    HWND hwnd = window.getSystemHandle();
                    std::string selectedFile = openFileDialog(hwnd);
                    spritesManager.loadSpriteSet(window, "tileSet.txt");
					spritesManager.loadEntitySet(window, "entitySet.txt");
					map.loadFromFile("map.txt"); //Eventually change "Map.txt" to selectedFile
                }
                else if (editorInterface.panelItems[i]->label.getString() == "Paint") {
                    std::cerr << "Paint button clicked" << std::endl;
                    editorState = PAINT;
                }
				else if (editorInterface.panelItems[i]->label.getString() == "Erase\nall") {
                    std::cerr << "Erase all button clicked" << std::endl;
                    editorState = ERASE_ALL;
				}
				else if (editorInterface.panelItems[i]->label.getString() == "Remove\ntiles") {
                    std::cerr << "Remove tiles button clicked" << std::endl;
                    editorState = REMOVE_TILES;
                }
                else if (editorInterface.panelItems[i]->label.getString() == "Remove\nactors") {
                    std::cerr << "Remove actors button clicked" << std::endl;
                    editorState = REMOVE_ACTORS;
                }
                else if (editorInterface.panelItems[i]->label.getString() == "Panel Tiles") {
                    std::cerr << "Switched to Tiles panel" << std::endl;
                    editorInterface.currentPanel = EditorPanel::Tiles;
                    editorInterface.updatePanel(spritesManager);
                }
                else if (editorInterface.panelItems[i]->label.getString() == "Panel Entities") {
                    std::cerr << "Switched to Entities panel" << std::endl;
                    editorInterface.currentPanel = EditorPanel::Entities;
                    editorInterface.updatePanel(spritesManager);
                }
                else if (editorInterface.panelItems[i]->label.getString() == "Deflt\nLayer") {
                    std::cerr << "Default layer button clicked" << std::endl;
                    editorInterface.layer = selectedSprite.defaultLayer;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " Lay\n er 0") {
                    std::cerr << "Layer 0 button clicked" << std::endl;
                    editorInterface.layer = 0;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " Lay\n er 1") {
                    std::cerr << "Layer 1 button clicked" << std::endl;
                    editorInterface.layer = 1;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " Lay\n er 2") {
                    std::cerr << "Layer 2 button clicked" << std::endl;
                    editorInterface.layer = 2;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " Lay\n er 3") {
                    std::cerr << "Layer 3 button clicked" << std::endl;
                    editorInterface.layer = 3;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " Lay\n er 4") {
                    std::cerr << "Layer 4 button clicked" << std::endl;
                    editorInterface.layer = 4;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " Lay\n er 5") {
                    std::cerr << "Layer 5 button clicked" << std::endl;
                    editorInterface.layer = 5;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " -") {
                    std::cerr << "Decrease brush size button clicked" << std::endl;
                    if (editorInterface.brushSize > 1) editorInterface.brushSize--;
                }
                else if (editorInterface.panelItems[i]->label.getString() == " +") {
                    std::cerr << "Increase brush size button clicked" << std::endl;
                    editorInterface.brushSize++;
                }
                else {
                    if (editorInterface.currentPanel == EditorPanel::Tiles) {
                        selectedSprite = *editorInterface.panelItems[i]->uniqueSprite;
                        editorInterface.layer = selectedSprite.defaultLayer;
                        editorInterface.outlineBox.shape.setPosition(editorInterface.panelItems[i]->shape.getPosition());
                    }
                    else {
                        selectedEntity = *editorInterface.panelItems[i]->uniqueEntity;
                        editorInterface.layer = selectedEntity.defaultLayer;
                        editorInterface.outlineBox.shape.setPosition(editorInterface.panelItems[i]->shape.getPosition());
                    }
                }
            }
            else if (editorInterface.panelItems[i]->blockEntityShape.getGlobalBounds().contains(mouseInterface)) {
                if (editorInterface.panelItems[i]->uniqueSprite != nullptr) {
                    editorInterface.panelItems[i]->uniqueSprite->blockEntity = !editorInterface.panelItems[i]->uniqueSprite->blockEntity;
                    // Mise à jour instantanée de la couleur du carré (Vert si true, Rouge si false)
                    editorInterface.panelItems[i]->blockEntityShape.setFillColor(
                        editorInterface.panelItems[i]->uniqueSprite->blockEntity ? sf::Color::Green : sf::Color::Red
                    );
                }
            }
            else if (editorInterface.panelItems[i]->blockLightShape.getGlobalBounds().contains(mouseInterface)) {
                if (editorInterface.panelItems[i]->uniqueSprite != nullptr) {
                    editorInterface.panelItems[i]->uniqueSprite->blockLight = !editorInterface.panelItems[i]->uniqueSprite->blockLight;
                    // Mise à jour instantanée de la couleur du carré (Vert si true, Rouge si false)
                    editorInterface.panelItems[i]->blockLightShape.setFillColor(
                        editorInterface.panelItems[i]->uniqueSprite->blockLight ? sf::Color::Green : sf::Color::Red
                    );
                }
            }
        }
    }

    
    if (!isInPanel) {
        if (editorState == PAINT) {
			if (editorInterface.currentPanel == EditorPanel::Tiles)
                map.setTile(selectedSprite, mouseWorld, editorInterface.layer, editorInterface.brushSize);
            else
				map.setEntity(selectedEntity, mouseWorld, editorInterface.layer);
            std::cerr << "painting" << std::endl;
        }
        if (editorState == ERASE_ALL) {
            map.resetTile(mouseWorld, editorInterface.brushSize);
            map.removeActor(mouseWorld, editorInterface.brushSize);
        }
        if (editorState == REMOVE_ACTORS) {
            map.removeActor(mouseWorld, editorInterface.brushSize);
        }
        if (editorState == REMOVE_TILES) {
            map.resetTile(mouseWorld, editorInterface.brushSize);
        }
    }
}


void EditorInputHandler::mouseWheelUp(sf::Vector2f mouseWorld, sf::Vector2f mouseInterface) {
    bool isInPanel = false;
    if (editorInterface.shape.getGlobalBounds().contains(mouseInterface)) {
        isInPanel = true;
    }

    if (isInPanel) {
        if (editorInterface.cursorOffset > 0) {
            editorInterface.cursorOffset--;
        }
    }
	else {
		camera.zoom(0.9f);
	}
}


void EditorInputHandler::mouseWheelDown(sf::Vector2f mouseWorld, sf::Vector2f mouseInterface) {
    bool isInPanel = false;
    if (editorInterface.shape.getGlobalBounds().contains(mouseInterface)) {
        isInPanel = true;
    }

    if (isInPanel) {
        if (editorInterface.cursorOffset < static_cast<int>(spritesManager.allUniqueTiles.size()) - 1) {
            editorInterface.cursorOffset++;
        }
    }
    else {
        camera.zoom(1.1f);
    }
}

void EditorInputHandler::cooldownUpdate() {
    if (cooldownLeft > 0) {
        cooldownLeft -= deltaTime;
    }
}







GameInputHandler::GameInputHandler(sf::RenderWindow& windowHere, Player& playerHere, Map& mapHere) : window(windowHere), player(playerHere), map(mapHere)
{
}

void GameInputHandler::gameInput()
{
    cooldownUpdate();
    sf::Event event;

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    // Loop to manage every event
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
        {
            std::cerr << "Closing the window" << std::endl;
            window.close();
        }
    }

    float moveX = 0;
    float moveY = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        std::cerr << "Going right" << std::endl;
        player.currentDirection = RIGHT;
        moveX = 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        std::cerr << "Going left" << std::endl;
        player.currentDirection = LEFT;
        moveX = -1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        std::cerr << "Going down" << std::endl;
        player.currentDirection = DOWN;
        moveY = 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        std::cerr << "Going up" << std::endl;
        player.currentDirection = UP;
        moveY = -1.f;
    }

    if (moveX != 0.f || moveY != 0.f)
    {
        // 1. Déplacement et test sur l'axe X
        player.move(moveX, 0.f);
        if (map.checkCollision(player.hitbox)) // Assure-toi d'avoir une méthode checkCollision dans Map
        {
            player.move(-moveX, 0.f); // Annule le mouvement X en cas de collision
        }

        // 2. Déplacement et test sur l'axe Y
        player.move(0.f, moveY);
        if (map.checkCollision(player.hitbox))
        {
            player.move(0.f, -moveY); // Annule le mouvement Y en cas de collision
        }
    }




    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        player.baseAttack(mouseWorld);
        std::cerr << "Base attack" << std::endl;
    }
    


}

void GameInputHandler::cooldownUpdate()
{
    if (cooldownLeft > 0) {
        cooldownLeft -= deltaTime;
    }
}
