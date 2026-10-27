#include "editor.hpp"


Editor::Editor(sf::RenderWindow& window) {
	//editorWindow = window;
    run(window);
	//Map map("map.txt");

	//window.draw(map);
}

void Editor::run(sf::RenderWindow& window) {
    Sprites_manager spritesManager;
    Map map(spritesManager);
    HWND hwnd = window.getSystemHandle();
    std::string selectedFile = openFileDialog(hwnd);
    spritesManager.newTile(window, selectedFile, 0, false, false);

    Unique_tile& selectedSprite = spritesManager.allUniqueTiles[0];
    Editor_interface editorInterface(spritesManager);
    sf::View camera = window.getDefaultView();
    sf::View interfaceView = window.getDefaultView();

    EditorInputHandler editorInputHandler(window, map, editorState, selectedSprite, editorInterface, spritesManager, camera);

    sf::Clock clock;

    while (window.isOpen() && BaseGameState == EDITING) {

        deltaTime = clock.restart().asSeconds();

        //std::cerr << "before the input() function" << std::endl;
        editorInputHandler.editorInput();
        //std::cerr << "after the input() function" << std::endl;

		editorInterface.updatePanel(spritesManager);

        window.clear(sf::Color::Black);
        window.setView(camera);
        map.displayMap(window);
        window.setView(interfaceView);
        editorInterface.displayPanel(window);
        window.display();
    }
}