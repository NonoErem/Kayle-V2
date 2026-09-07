
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







EditorInputHandler::EditorInputHandler(sf::RenderWindow& windowHere, Map& mapHere, EditorState editorStateHere, Unique_sprite& selectedSpriteHere, Editor_interface& editorInterfaceHere, Sprites_manager& spritesManagerHere, sf::View& cameraHere) : window(windowHere), map(mapHere), editorState(editorStateHere), selectedSprite(selectedSpriteHere), editorInterface(editorInterfaceHere), spritesManager(spritesManagerHere), camera(cameraHere) {

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
            if (editorInterface.panelItems[i].shape.getGlobalBounds().contains(mouseInterface)) {

                if (editorInterface.panelItems[i].label.getString() == "Add tile") {
					std::cerr << "Add tile button clicked" << std::endl;
                    HWND hwnd = window.getSystemHandle();
                    std::string selectedFile = openFileDialog(hwnd);
                    int layer = 0;
                    layer = spritesManager.askDefaultLayer(window);
                    spritesManager.newSprite(window, selectedFile, layer);

				}
                else if (editorInterface.panelItems[i].label.getString() == "Save") {
					std::cerr << "Save button clicked" << std::endl;
					map.saveToFile("map.txt");
                    spritesManager.saveSpriteSet("spriteSet.txt");
				}
                else if (editorInterface.panelItems[i].label.getString() == "Load") {
					std::cerr << "Load button clicked" << std::endl;
                    HWND hwnd = window.getSystemHandle();
                    std::string selectedFile = openFileDialog(hwnd);
                    spritesManager.loadSpriteSet(window, "spriteSet.txt");
					map.loadFromFile("map.txt"); //Eventually change "Map.txt" to selectedFile
                }
                else if (editorInterface.panelItems[i].label.getString() == "Paint") {
                    std::cerr << "Paint button clicked" << std::endl;
                    editorState = PAINT;
                }
				else if (editorInterface.panelItems[i].label.getString() == "Erase\nall") {
                    std::cerr << "Erase all button clicked" << std::endl;
                    editorState = ERASE_ALL;
				}
				else if (editorInterface.panelItems[i].label.getString() == "Remove\ntiles") {
                    std::cerr << "Remove tiles button clicked" << std::endl;
                    editorState = REMOVE_TILES;
				}
                else if (editorInterface.panelItems[i].label.getString() == "Remove\nactors") {
					std::cerr << "Remove actors button clicked" << std::endl;
                    editorState = REMOVE_ACTORS;
                }
                else if (editorInterface.panelItems[i].label.getString() == " Lay\n er 0") {
					std::cerr << "Layer 0 button clicked" << std::endl;
                    editorInterface.layer = 0;
                }
                else if (editorInterface.panelItems[i].label.getString() == " Lay\n er 1") {
                    std::cerr << "Layer 1 button clicked" << std::endl;
                    editorInterface.layer = 1;
                }
                else if (editorInterface.panelItems[i].label.getString() == " Lay\n er 2") {
                    std::cerr << "Layer 2 button clicked" << std::endl;
                    editorInterface.layer = 2;
                }
                else if (editorInterface.panelItems[i].label.getString() == " Lay\n er 3") {
                    std::cerr << "Layer 3 button clicked" << std::endl;
                    editorInterface.layer = 3;
                }
                else if (editorInterface.panelItems[i].label.getString() == " Lay\n er 4") {
                    std::cerr << "Layer 4 button clicked" << std::endl;
                    editorInterface.layer = 4;
                }
                else if (editorInterface.panelItems[i].label.getString() == " Lay\n er 5") {
                    std::cerr << "Layer 5 button clicked" << std::endl;
                    editorInterface.layer = 5;
                }
                else {
                    selectedSprite = editorInterface.panelItems[i].uniqueSprite;
                    editorInterface.layer = selectedSprite.defaultLayer;
					editorInterface.outlineBox.shape.setPosition(editorInterface.panelItems[i].shape.getPosition());
                }
            }
        }
    }

    
    if (!isInPanel) {
        if (editorState == PAINT) {
            map.setTile(selectedSprite, mouseWorld, editorInterface.layer);
            std::cerr << "painting" << std::endl;
        }
        if (editorState == ERASE_ALL) {
            map.resetTile(mouseWorld);
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
        if (editorInterface.cursorOffset < static_cast<int>(spritesManager.allUniqueSprites.size()) - 1) {
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