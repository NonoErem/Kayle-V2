#pragma once
#include <SFML/Graphics.hpp>
#include "sprites_manager.hpp"
#include "map.hpp"
#include "globalvariables.hpp"
#include "editor_interface.hpp"
#include "inputHandler.hpp"
#include "editorState.hpp"


class Editor
{
public:
	Editor(sf::RenderWindow& window); //Make sure to initialize the editor with a map and interface
	void run(sf::RenderWindow& window);
	EditorState editorState = PAINT;
	//displayInterface(); //Probably from the Interface class
private:
	//sf::RenderWindow& editorWindow;
	//map
};

