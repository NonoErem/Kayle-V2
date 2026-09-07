#include <SFML/Graphics.hpp>
#include "menu_old.hpp"
#include "map_old.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "config.hpp"
#include "input_old.hpp"
#include "actor.hpp"
#include "light.hpp"
#include "editor_old.hpp"
#include "interface_old.hpp"
#include <vector>

#include <iostream>

sf::Vector2f cameraPos;
bool cameraInitialized = false;



//##################################################################
//##################### COLLISIONS WITH ACTORS #####################
//##################################################################

static void actorCollisions(const std::vector<std::unique_ptr<Actor>>& entities) {
    // Vérifier collisions entre acteurs
    for (int i = 0; i < entities.size(); ++i) {
        auto actor1 = dynamic_cast<Actor*>(entities[i].get());
        if (!actor1) continue;

        sf::FloatRect hitbox1 = actor1->getHitBox();

        for (int j = i + 1; j < entities.size(); ++j) {
            auto actor2 = dynamic_cast<Actor*>(entities[j].get());
            if (!actor2) continue;

            sf::FloatRect hitbox2 = actor2->getHitBox();

            if (hitbox1.intersects(hitbox2) && dynamic_cast<Entity*>(actor1) && dynamic_cast<Entity*>(actor2)) {
                // Correction améliorée : repousser les deux ennemis l'un de l'autre

                // Calculer la direction entre les deux ennemis
                sf::Vector2f dir = actor2->getPosition() - actor1->getPosition();

                if (dir.x == 0 && dir.y == 0)
                    dir.x = 1.f; // éviter un vecteur nul

                float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                dir /= length;

                // Calculer le chevauchement (overlap)
                float overlap = (32.f * 0.8f) - length; // hitbox ~80% TILE_SIZE

                if (overlap > 0.f) {
                    // Repousser les deux ennemis de moitié chacun
                    sf::Vector2f pushVec = dir * (overlap / 2.f);

                    sf::Vector2f newPos1 = actor1->getPosition() - pushVec;
                    sf::Vector2f newPos2 = actor2->getPosition() + pushVec;

                    actor1->setPosition(newPos1.x, newPos1.y);
                    actor2->setPosition(newPos2.x, newPos2.y);
                }
            }


            // Collisions entre un projectile et une entité (joueur, ennemi ou allié)

            if (hitbox1.intersects(hitbox2)) {

                auto spell = dynamic_cast<Spell*>(actor1);
                auto meleeAttack = dynamic_cast<MeleeAttack*>(actor1);
                auto proj = dynamic_cast<Projectile*>(actor1);
                auto firstSpell = dynamic_cast<FirstSpell*>(actor1);

                auto entity = dynamic_cast<Entity*>(actor2);

                if (spell && entity && (spell->team != entity->team))
                {
                    std::cout << "Ennemi TOUCHEEEEEEEE" << std::endl;

                    if (proj) {
                        entity->HP -= proj->damage; // infliger des dégâts
                        proj->isAlive = false; // détruire le projectile
                    }
                    if (meleeAttack) {
                        entity->HP -= meleeAttack->damage; // infliger des dégâts
                        actor2->cannotMoveFor = 0.3f; // empêcher le mouvement temporairement
                        meleeAttack->isAlive = false; // fin de l'attaque
                    }
                    if (firstSpell) {
                        std::cout << "Ennemi RALENTIIIII" << std::endl;

                        entity->slowedBy = 0.70f; // ralentir l'entité  
                        entity->slowDurationLeft = 2.0f; // durée du ralentissement
                    }
                }

                spell = dynamic_cast<Spell*>(actor2);
                proj = dynamic_cast<Projectile*>(actor2);
                meleeAttack = dynamic_cast<MeleeAttack*>(actor2);
                firstSpell = dynamic_cast<FirstSpell*>(actor2);

                entity = dynamic_cast<Entity*>(actor1);

                if (spell && entity && (spell->team != entity->team))
                {
                    std::cout << "Ennemi TOUCHEEEEEEEE" << std::endl;

                    if (proj) {
                        entity->HP -= proj->damage; // infliger des dégâts
                        proj->isAlive = false; // détruire le projectile
                    }
                    if (meleeAttack) {
                        entity->HP -= meleeAttack->damage; // infliger des dégâts
                        actor1->cannotMoveFor = 0.3f; // empêcher le mouvement temporairement
                        meleeAttack->isAlive = false; // fin de l'attaque
                    }
                    if (firstSpell) {
                        std::cout << "Ennemi RALENTIIIII" << std::endl;

                        entity->slowedBy = 0.70f; // ralentir l'entité  
                        entity->slowDurationLeft = 2.0f; // durée du ralentissement
                    }
                }


                // MeleeEnemy vs Player collision

                auto player = dynamic_cast<Player*>(actor1);
                auto meleeEnemy = dynamic_cast<MeleeEnemy*>(actor2);

                if (player && meleeEnemy && player->vulnerable)
                {
                    player->HP -= meleeEnemy->damage; // infliger des dégâts
                    player->vulnerable = false; // rendre le joueur invulnérable
                    player->vulnerabilityCooldown = 0.5; // 0.5 secondes d'invulnérabilité
                }

                player = dynamic_cast<Player*>(actor2);
                meleeEnemy = dynamic_cast<MeleeEnemy*>(actor1);

                if (player && meleeEnemy && player->vulnerable)
                {
                    player->HP -= meleeEnemy->damage; // infliger des dégâts
                    player->vulnerable = false; // rendre le joueur invulnérable
                    player->vulnerabilityCooldown = 0.5; // 0.5 secondes d'invulnérabilité
                }
            }
        }
    }

}




//##################################################################
//###################### COLLISIONS WITH MAP #######################
//##################################################################

static void mapCollisions(const std::vector<std::unique_ptr<Actor>>& entities, Map_old& map) {
    // Collision décor pour chaque acteur
    for (const auto& e : entities) {
        Actor* actor = e.get();
        if (!actor)
            continue;

        // On fera une logique différente pour les spells
        if (dynamic_cast<Spell*>(actor))
            continue;

        sf::FloatRect hitbox = actor->getHitBox();

        // Vérifier les cases touchées par la hitbox
        int leftTile = static_cast<int>(hitbox.left) / (TILE_SIZE * VIEW_SCALE);
        int rightTile = static_cast<int>(hitbox.left + hitbox.width) / (TILE_SIZE * VIEW_SCALE);
        int topTile = static_cast<int>(hitbox.top) / (TILE_SIZE * VIEW_SCALE);
        int bottomTile = static_cast<int>(hitbox.top + hitbox.height) / (TILE_SIZE * VIEW_SCALE);

        for (int ty = topTile; ty <= bottomTile; ++ty) {
            for (int tx = leftTile; tx <= rightTile; ++tx) {
                if (map.isSolidTile(tx, ty)) {
                    // Récupérer le rectangle du tile
                    sf::FloatRect tileRect(
                        tx * TILE_SIZE * VIEW_SCALE,
                        ty * TILE_SIZE * VIEW_SCALE,
                        TILE_SIZE * VIEW_SCALE,
                        TILE_SIZE * VIEW_SCALE
                    );

                    if (hitbox.intersects(tileRect)) {
                        // Simple correction : repousser l'acteur hors du tile
                        sf::FloatRect intersection;
                        hitbox.intersects(tileRect, intersection);

                        sf::Vector2f pos = actor->getPosition();

                        if (intersection.width < intersection.height) {
                            if (hitbox.left < tileRect.left)
                                pos.x -= intersection.width;
                            else
                                pos.x += intersection.width;
                        }
                        else {
                            if (hitbox.top < tileRect.top)
                                pos.y -= intersection.height;
                            else
                                pos.y += intersection.height;
                        }

                        actor->setPosition(pos.x, pos.y);
                        hitbox = actor->getHitBox(); // mettre à jour hitbox après déplacement
                    }
                }
            }
        }

        // Bordures de la map
        float mapWidthPx = map.MAP_WIDTH * TILE_SIZE * VIEW_SCALE;
        float mapHeightPx = map.MAP_HEIGHT * TILE_SIZE * VIEW_SCALE;

        hitbox = actor->getHitBox(); // à jour après corrections tiles
        sf::Vector2f pos = actor->getPosition();

        // Calcul de l'offset hitbox -> position (pour repositionner correctement)
        float offsetX = hitbox.left - pos.x;
        float offsetY = hitbox.top - pos.y;

        if (hitbox.left < 0.f)
            pos.x = -offsetX;
        if (hitbox.top < 0.f)
            pos.y = -offsetY;
        if (hitbox.left + hitbox.width > mapWidthPx)
            pos.x = mapWidthPx - hitbox.width - offsetX;
        if (hitbox.top + hitbox.height > mapHeightPx)
            pos.y = mapHeightPx - hitbox.height - offsetY;

        actor->setPosition(pos.x, pos.y);

    }
}


//##################################################################
//####################### CAMERA MANAGEMENT ########################
//##################################################################

sf::View cameraManagement(sf::View camera, Player* player, sf::RenderWindow& window, Map_old& map) {
    // -------------------------
    // Paramètres dead zone
    // -------------------------

    // Taille de la dead zone en tiles
    int deadZoneTilesX = 3;
    int deadZoneTilesY = 2;

    // Taille en pixels
    float deadZoneHalfWidth = (deadZoneTilesX * TILE_SIZE) / 2.f;
    float deadZoneHalfHeight = (deadZoneTilesY * TILE_SIZE) / 2.f;

    // -------------------------
    // Position du joueur
    // -------------------------

    sf::Vector2f playerPos = player->getPosition();

    // Initialisation une seule fois
    if (!cameraInitialized) {
        cameraPos = playerPos;
        cameraInitialized = true;
    }


    // -------------------------
    // Calcul des bords de la dead zone
    // -------------------------

    float left = cameraPos.x - deadZoneHalfWidth;
    float right = cameraPos.x + deadZoneHalfWidth;
    float top = cameraPos.y - deadZoneHalfHeight;
    float bottom = cameraPos.y + deadZoneHalfHeight;

    // -------------------------
    // Déplacement caméra si joueur sort
    // -------------------------

    sf::Vector2f targetPos = cameraPos;

    // Horizontal

    targetPos.x = playerPos.x;

    // Vertical
    targetPos.y = playerPos.y;

    // -------------------------
    // Lissage de la caméra
    // -------------------------

    float smoothingFactor = 0.005f; // entre 0.01 et 0.2
    cameraPos += (targetPos - cameraPos) * smoothingFactor;

    // -------------------------
    // Clamp la caméra dans la map
    // -------------------------

    float mapWidthPx = map.MAP_WIDTH * TILE_SIZE * VIEW_SCALE;
    float mapHeightPx = map.MAP_HEIGHT * TILE_SIZE * VIEW_SCALE;

    float halfWidthPx = SCREEN_WIDTH / 2.0f;
    float halfHeightPx = SCREEN_HEIGHT / 2.0f;

    cameraPos.x = std::max(halfWidthPx, std::min(cameraPos.x, mapWidthPx - halfWidthPx));
    cameraPos.y = std::max(halfHeightPx, std::min(cameraPos.y, mapHeightPx - halfHeightPx));
    //cameraPos.x = std::min(cameraPos.x, mapWidthPx - halfWidthPx);
    //cameraPos.y = std::min(cameraPos.y, mapHeightPx - halfHeightPx);




    // -------------------------
    // Appliquer à la vue
    // -------------------------

    camera.setCenter(cameraPos);
    window.setView(camera);
    return camera;
}





//##################################################################
//########################## GAME STARTS ###########################
//##################################################################

void lancerJeu() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kayle");

    Map_old map;
    if (!map.loadFromFile("map.txt")) {
        return;
    }
    std::vector<Edge> edges = generateEdgesFromMap(map);


    LightSystem lights(map.MAP_WIDTH * TILE_SIZE * VIEW_SCALE, map.MAP_HEIGHT * TILE_SIZE * VIEW_SCALE);

    Player* player = map.getPlayer();
    Interface_old interface(player);
    sf::Vector2f lightPosition = sf::Vector2f(player->getPosition().x + (TILE_SIZE * VIEW_SCALE) / 2, player->getPosition().y + (TILE_SIZE * VIEW_SCALE) / 2);
    Light playerLight(lightPosition, 400.f);

    // Lumière texture
    sf::Texture runeTexture;
    runeTexture.loadFromFile("Textures/effects/waves 1.png");
    Light runeLight({ 600, 400 }, 300.f, Light::Type::Texture);
    runeLight.setTexture(&runeTexture);

    std::vector<std::unique_ptr<Actor>>& entities = map.getActors();
    sf::Clock clock;
    sf::Clock fpsClock;   // pour limiter la fréquence d'affichage
    int frameCount = 0;


    sf::View camera(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

    while (window.isOpen()) {

        sf::Event event;

        float scrollDelta = 0.f;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelScrolled) {
                scrollDelta += event.mouseWheelScroll.delta;
            }
        }

        float deltaTime = clock.restart().asSeconds();
        if (deltaTime > 0.1f) deltaTime = 0.1f; // évite les gros sauts de fps
        frameCount++;

        // Afficher FPS toutes les secondes
        if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
            float fps = frameCount / fpsClock.restart().asSeconds();
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
        }
        // Input spécifique au joueur
        playerInput(*player, deltaTime, map, window, scrollDelta, camera);


        std::vector<Actor*> newEntities;

        // Update général
        for (auto& e : entities) {
            if (!e) continue;

            e->update(deltaTime);

            if (auto ranged = dynamic_cast<RangedEnemy*>(e.get())) {
                newEntities.push_back(ranged);
            }

        }

        for (auto* e : newEntities) {
            e->sendProjectile(map, window); // Envoi de projectiles pour les ennemis à distance
        }

        entities.erase(
            std::remove_if(
                entities.begin(),
                entities.end(),
                [](const std::unique_ptr<Actor>& actor) {
                    if (!actor->isAlive) {
                        std::cout << actor->getId() << " supprimé\n";
                        return true;
                    }
                    return false;
                }),
            entities.end()
        );


        actorCollisions(entities);


        mapCollisions(entities, map);


        camera = cameraManagement(camera, player, window, map);



        window.clear();

        window.setView(camera);
        sf::Vector2f playerPos = player->getPosition();
        map.drawLayer(window, 0, playerPos);
        map.drawLayer(window, 1, playerPos);
        map.drawLayer(window, 2, playerPos);


        for (const auto& e : entities) {
            e->draw(window);
        }

        map.drawLayer(window, 3, playerPos);

        lights.clearLights();
        sf::Vector2f lightPosition = sf::Vector2f(player->getPosition().x + (TILE_SIZE * VIEW_SCALE) / 4, player->getPosition().y + (TILE_SIZE * VIEW_SCALE) / 4);
        playerLight.setPosition(lightPosition);
        runeLight.setPosition(lightPosition);
        lights.addLight(playerLight);
        //lights.addLight(runeLight);


        lights.render(window, edges);

        map.drawLayer(window, 4, playerPos, sf::Color(100, 100, 100));

        window.setView(window.getDefaultView());
        interface.update(player);
        interface.draw(window);

        window.display();

    }
}











//##################################################################
//############################## MAIN ##############################
//##################################################################

int main_old() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Kayle");
    GameState state = GameState::MENU;
    Menu menu(window.getSize().x, window.getSize().y);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (state == GameState::MENU) {
                menu.handleInput(event, state);
            }
        }

        if (state == GameState::EXIT) {
            window.close();
        }
        else if (state == GameState::MENU) {
            window.clear();
            menu.draw(window);
            window.display();
        }
        else if (state == GameState::PLAYING) {
            lancerJeu();
            // Après retour de lancerJeu, on revient au menu
            state = GameState::MENU;
        }
        else if (state == GameState::EDITOR) {
            lancerEditeur(window, state);
        }
    }

    return 0;
}
