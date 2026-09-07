#include "input_old.hpp"
#include "player.hpp"
#include "config.hpp"
#include <iostream>

void playerInput(Player& player, float dt, Map_old& map, const sf::RenderWindow& window, float scrollDelta, sf::View& camera) {
    sf::Vector2f movement(0.f, 0.f);
    if (player.meleeAnimationCooldown<=0) {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            player.currentDirection = LEFT;
            movement.x -= player.getEffectiveSpeed() * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            player.currentDirection = RIGHT;
            movement.x += player.getEffectiveSpeed() * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            player.currentDirection = UP;
            movement.y -= player.getEffectiveSpeed() * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.currentDirection = DOWN;
            movement.y += player.getEffectiveSpeed() * dt;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            if (player.attackCooldown <= 0.f)
            {
                if (player.level == 1) {
                    std::cout << "MeleeWeapon\n";
                    player.meleeWeapon(map, window);
                    player.attackCooldown = player.meleeRate;
					player.meleeAnimationCooldown = player.meleeAnimationDuration;
                }
                if (player.level == 2) {
                    std::cout << "RangeWeapon\n";
                    player.sendProjectile(map, window);
                    player.attackCooldown = player.fireRate;
                }
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (player.firstSpellCooldown <= 0.f)
            {
                if (player.level == 1) {
                    player.sendFirstSpell(map, window);
                    player.firstSpellCooldown = player.firstSpellRate;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            if (player.secondSpellCooldown <= 0.f)
            {
                if (player.level == 1) {
					player.quickenedBy = 0.4f; // Ratio d'augmentation de la vitesse
                    player.quickenedDurationLeft = 2.f;
                    player.secondSpellCooldown = player.secondSpellRate;

                }
            }
        }

        

    }
    else {
        player.meleeAnimationCooldown -= dt;
    }

	// Utilisation de la molette de la souris pour zoomer
    if (scrollDelta > 0.f) {
        camera.zoom(0.9f); // Zoom avant
    }
    else if (scrollDelta < 0.f) {
        camera.zoom(1.1f); // Zoom arrière
    }

	player.move(movement);
    // Appliquer le déplacement
}


