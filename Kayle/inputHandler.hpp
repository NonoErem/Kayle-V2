#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include "menu.hpp"
#include "editor_interface.hpp"
#include "editorState.hpp"


enum class Action {
	Up,
	Down,
	Select
};

class InputHandler {
public:
	InputHandler();
	void bindKey(Action action, sf::Keyboard::Key key); //Add a key
	void setKeys(Action action, const std::vector<sf::Keyboard::Key>& keys); //Change the keys
	void letPlayerAddKey(Action action, sf::RenderWindow& window); //Let the player add a key

	bool isActionPressed(Action action, sf::Keyboard::Key key) const;
	void input(sf::RenderWindow& window, Menu& currentMenu);
	void moveUp(Menu& currentMenu);
	void moveDown(Menu& currentMenu);
	void cooldownUpdate();
private:
	float cooldown = 0.1f;
	float cooldownLeft = 0.1f; //Don't set its base value on 0
	Menu& currentMenu;
	std::unordered_map<Action, std::vector<sf::Keyboard::Key>> keyBindings;
};

class EditorInputHandler {
public:
	EditorInputHandler(sf::RenderWindow& window, Map& map, EditorState editorState, Unique_sprite& selectedSprite, Editor_interface& editorInterface, Sprites_manager& spritesManager, sf::View& cameraHere);
	void editorInput();

	void leftClick(sf::Vector2f mousPos, sf::Vector2f mouseInterface);
	void mouseWheelUp(sf::Vector2f mousePos, sf::Vector2f mouseInterface);
	void mouseWheelDown(sf::Vector2f mousePos, sf::Vector2f mouseInterface);
	void cooldownUpdate();


private:
	sf::RenderWindow& window;
	Map& map;
	EditorState editorState;
	Unique_sprite selectedSprite;
	Editor_interface& editorInterface;
	Sprites_manager& spritesManager;
	sf::View& camera;

	float cooldown = 0.0f; //I keep it on 0 until I change my mind
	float cooldownLeft = 0.1f; //Don't set its base value on 0
};




