#include "game.hpp"

Game::Game(sf::RenderWindow& window) {
	run(window);
}

void Game::run(sf::RenderWindow& window) {
	
	sf::Clock clock;


	Sprites_manager spritesManager;
	Map map(spritesManager);
	spritesManager.loadSpriteSet(window, "tileSet.txt");
	spritesManager.loadEntitySet(window, "entitySet.txt");
	spritesManager.loadEffectSet(window, "effectSet.txt");
	std::cout << "effect set size :" << static_cast<int>(spritesManager.allUniqueEffects.size()) << std::endl;

	map.loadFromFile("map.txt");
	Player player(map);
	GameInputHandler gameInputHandler(window, player, map);

	sf::View camera = window.getDefaultView();

	camera.zoom(0.5f);

	while (window.isOpen() && BaseGameState == PLAYING) {
		window.clear(sf::Color::Black);
		
		deltaTime = clock.restart().asSeconds();

		
		window.setView(camera);
		gameInputHandler.gameInput();
		camera.setCenter(player.posX,player.posY);
		player.playerUpdate();
		map.updateEntities(deltaTime);
		map.updateEffects(deltaTime);
		map.displayMap(window, player.sprite);
		//window.draw(player.sprite);
		window.display();
	}
}