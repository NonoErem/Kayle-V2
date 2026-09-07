#include <SFML/Graphics.hpp>
#include "menu_old.hpp"
#include "map_old.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "config.hpp"
#include "input_old.hpp"
#include "actor.hpp"
#include "light.hpp"
#include "textures.hpp"
#include "editor_old.hpp"
#include "editorpanel.hpp"
#include <vector>

#include <iostream>






//##################################################################
//######################## EDITOR STARTS ###########################
//##################################################################


void lancerEditeur(sf::RenderWindow& window, GameState& state) {

    Map_old map;
    if (!map.loadFromFile("map.txt")) {
        return;
    }

    Player* player = map.getPlayer();


    std::vector<std::unique_ptr<Actor>>& entities = map.getActors();

    float clickIsReadyIn = 0.2f; // Pour éviter les clics multiples

    sf::View camera = window.getDefaultView();

    const float cameraSpeed = 300.f; // pixels/second

    sf::Clock clock;



    // ---------------------------------------
    // Initialisation des panneaux
    // ---------------------------------------



    float panelWidth = 200.f;
    float panelHeight = 200.f;

    EditorPanel tilesetPanel(0.1f * window.getSize().x, 0.1f * window.getSize().y, panelWidth, panelHeight);


	vector panels = { &tilesetPanel };



    // ---------------------------------------
    // Barre de sélection de texture
    // ---------------------------------------
    const float PANEL_WIDTH = 200.f;
    float scrollOffset = 0.f; // position du scroll en pixels
    const float SCROLL_SPEED = 50.f; // vitesse de défilement

    std::map<int, TileInfo> tilePaths = texturesInfo();
    std::vector<sf::Texture> tileTextures(tilePaths.size());
    std::vector<sf::Sprite> tileIcons;

	std::map<int, ActorInfo> actorPaths = actorsInfo();
    std::vector<sf::Texture> actorTextures(actorPaths.size());
	std::vector<sf::Sprite> actorIcons;

	// Chargement dynamique des textures des tuiles

    for (int i = 0; i < tilePaths.size(); ++i) {
        std::cout << i << " -> " << tilePaths[i].texturePath << "\n";
        if (!tileTextures[i].loadFromFile(tilePaths[i].texturePath)) continue;
        sf::Sprite sprite(tileTextures[i]);
        sprite.setScale(1.2f, 1.2f);
        sprite.setPosition(0.f, i * 50.f); // position relative (pas finale)
        tileIcons.push_back(sprite);
    }

	// Chargement dynamique des textures des acteurs

    for (int i = 0; i < actorPaths.size(); ++i) {
        std::cout << i << " -> " << actorPaths[i].texturePath << "\n";
        if (!actorTextures[i].loadFromFile(actorPaths[i].texturePath)) continue;
        sf::Sprite sprite(actorTextures[i]);
        sprite.setScale(1.2f, 1.2f);
        sprite.setPosition(0.f, i * 50.f); // position relative (pas finale)
        actorIcons.push_back(sprite);
    }

    while (window.isOpen() && state == GameState::EDITOR) {
        float dt = clock.restart().asSeconds();


        auto isMouseInPanel = [&](sf::Vector2i mousePos) {
            return mousePos.x > (int)(window.getSize().x - PANEL_WIDTH);
            };


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    state = GameState::MENU;
                }
                if (event.key.code == sf::Keyboard::O) {
                    map.saveToFile("map.txt");
                }
                if (event.key.code == sf::Keyboard::R) {
                    map.resetMap();
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (isMouseInPanel(sf::Mouse::getPosition(window))) {
                    scrollOffset -= event.mouseWheelScroll.delta * SCROLL_SPEED;

                    // Empêche le scroll de sortir des limites
                    float maxScroll = std::max(0.f, (float)tileIcons.size() * 50.f - window.getSize().y);
                    scrollOffset = std::clamp(scrollOffset, 0.f, maxScroll);
				}
                else {
                    if (event.mouseWheelScroll.delta > 0) {
                        camera.zoom(0.9f);
                    }
                    else if (event.mouseWheelScroll.delta < 0) {
                        camera.zoom(1.1f);
                    }
                }
            }
        }

        //---------------------------------------
        // Déplacement caméra au clavier
        //---------------------------------------
        sf::Vector2f cameraOffset(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            cameraOffset.x -= cameraSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            cameraOffset.x += cameraSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            cameraOffset.y -= cameraSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            cameraOffset.y += cameraSpeed * dt;
        }

        camera.move(cameraOffset);



        //---------------------------------------
        // Pose d’une tuile à la souris
        //---------------------------------------


        if (clickIsReadyIn > 0.f) {
            clickIsReadyIn -= dt;
        }


        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // Clic gauche dans le panneau : changer la texture
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            int highestLayer = 0;
            EditorPanel selectedPanel;
            for (auto& panel : panels) {
                if (panel->mouseInPanel(mousePos)) {
                    if (panel->getLayer() > highestLayer) {
                        highestLayer = panel->getLayer();
						selectedPanel = *panel;
                    }
                    std::cout << "Mouse is in panel\n";
                }
            }


            if (clickIsReadyIn <= 0.f) {
                tilesetPanel.getLeftClickPosition(mousePos);
				clickIsReadyIn = 0.2f; // Réinitialiser le délai pour éviter les clics multiples
            }

            
			
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            window.setView(camera);

            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

            float radius = TILE_SIZE;
            map.removeActorAt(worldPos, radius);
        }




        //---------------------------------------
        // Rendu
        //---------------------------------------
        window.clear();

        window.setView(camera);
        map.drawLayer(window, 0, camera.getCenter());
        map.drawLayer(window, 1, camera.getCenter());
        map.drawLayer(window, 2, camera.getCenter());
        map.drawLayer(window, 3, camera.getCenter());
        map.drawLayer(window, 4, camera.getCenter());



        for (const auto& e : entities) {
            e->draw(window);
        }


        //##################################################################
        //####################### RENDU INTERFACE ##########################
        //##################################################################

        window.setView(window.getDefaultView());


		tilesetPanel.draw(window);
        // --- Rendu du panneau ---

        


        window.display();
    }
}