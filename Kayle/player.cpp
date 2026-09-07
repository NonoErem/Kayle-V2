// player.cpp
#include "player.hpp"
#include "config.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "projectile.hpp"
#include "map_old.hpp"
#include <iostream>



Player::Player() {
    
    // Charge les deux textures pour chaque direction

    textureIdle[0].loadFromFile("Textures/player/back_player_A.png");
    textureIdle[1].loadFromFile("Textures/player/back_player_A.png");


	// Pour avoir le perso en x64, il faut que la première texture utilisée soit en x64
    textureDown[0].loadFromFile("Textures/player/front_player_A.png");
    textureDown[1].loadFromFile("Textures/player/front_player_A.png");

    textureUp[0].loadFromFile("Textures/player/back_player_A.png");
    textureUp[1].loadFromFile("Textures/player/back_player_A.png");

    textureLeft[0].loadFromFile("Textures/player/left_player_A.png");
    textureLeft[1].loadFromFile("Textures/player/left_player_A.png");

    textureRight[0].loadFromFile("Textures/player/right_player_A.png");
    textureRight[1].loadFromFile("Textures/player/right_player_A.png");

    // Textures d'attaque
    textureMeleeDown[0].loadFromFile("Textures/player/front_player_A.png");
    textureMeleeDown[1].loadFromFile("Textures/player/front_player_A.png");

    textureMeleeUp[0].loadFromFile("Textures/player/back_player_A.png");
    textureMeleeUp[1].loadFromFile("Textures/player/back_player_A.png");

    textureMeleeLeft[0].loadFromFile("Textures/player/melee_left_player_A.png");
    textureMeleeLeft[1].loadFromFile("Textures/player/melee_left_player_B.png");

    textureMeleeRight[0].loadFromFile("Textures/player/melee_right_player_A.png");
    textureMeleeRight[1].loadFromFile("Textures/player/melee_right_player_B.png");


    
    // Taille et couleur héritées de Entity constructor

    sprite.setTexture(textureIdle[0]);
    sprite.setPosition(100.f, 100.f);
//    sprite.setScale(0.5f * VIEW_SCALE, 0.5f * VIEW_SCALE);


    speed = 200.f;
    id = "pl";
	maxHP = 100.f; // Points de vie maximum du joueur
    HP = maxHP; // Points de vie du joueur
    team = 0;
    vulnerable = true;
}

void Player::changeTexture(float dt) {

    int frameIndex = useFirstTexture ? 1 : 0;

    switch (currentDirection) {
    case DOWN:
        sprite.setTexture(textureDown[frameIndex]);
        break;
    case UP:
        sprite.setTexture(textureUp[frameIndex]);
        break;
    case LEFT:
        sprite.setTexture(textureLeft[frameIndex]);
        break;
    case RIGHT:
        sprite.setTexture(textureRight[frameIndex]);
        break;

    case DOWN_MELEE:
        sprite.setTexture(textureMeleeDown[frameIndex]);
        break;
    case UP_MELEE:
        sprite.setTexture(textureMeleeUp[frameIndex]);
        break;
    case LEFT_MELEE:
        sprite.setTexture(textureMeleeLeft[frameIndex]);
        break;
    case RIGHT_MELEE:
        sprite.setTexture(textureMeleeRight[frameIndex]);
        break;

    default:
        sprite.setTexture(textureIdle[frameIndex]);
        break;
    }
}

int Player::getEffectiveSpeed() {
    return effectiveSpeed;
}


std::string Player::getId() const {
    return id;
}
void Player::meleeWeapon(Map_old& map, const sf::RenderWindow& window) {
    sf::Vector2f playerPos = this->sprite.getPosition();

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    sf::Vector2f startPosition = this->sprite.getPosition();
    sf::Vector2f direction = mouseWorld - startPosition;

    std::unique_ptr<MeleeAttack> projectile = std::make_unique<MeleeAttack>(this->getPosition(), direction, this->team);


    sf::Vector2f projectileSize(projectile->getHitBox().width, projectile->getHitBox().height);
    float offsetX = (sprite.getGlobalBounds().width - projectileSize.x) / 2.f;
    float offsetY = (sprite.getGlobalBounds().height - projectileSize.y) / 2.f;
    if (abs(mouseWorld.x - playerPos.x) > abs(mouseWorld.y - playerPos.y)) {
        if (mouseWorld.x > playerPos.x) {
            offsetX += TILE_SIZE / 2.f;
            currentDirection = RIGHT_MELEE;
        }
        else {
            offsetX -= TILE_SIZE / 2.f;
            currentDirection = LEFT_MELEE;
        }
    }
    else {
        if (mouseWorld.y > playerPos.y) {
            offsetY += TILE_SIZE / 2.f;
            currentDirection = DOWN_MELEE;
        }
        else {
            offsetY -= TILE_SIZE / 2.f;
            currentDirection = UP_MELEE;
        }
    }
    projectile->setPosition(startPosition.x + offsetX, startPosition.y + offsetY);

    map.addActor(std::move(projectile));
}

void Player::sendProjectile(Map_old& map, const sf::RenderWindow& window) {  
   // Créer un projectile à la position actuelle du joueur  
   sf::Vector2f playerPos = this->sprite.getPosition();  

   sf::Vector2i mousePixel = sf::Mouse::getPosition(window);  
   sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);  

   sf::Vector2f startPosition = this->sprite.getPosition();  
   sf::Vector2f direction = mouseWorld - startPosition;  

   std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(this->getPosition(), direction, this->team, damage);  

   sf::Vector2f projectileSize(projectile->getHitBox().width, projectile->getHitBox().height);  
   float offsetX = (sprite.getGlobalBounds().width - projectileSize.x) / 2.f;
   float offsetY = (sprite.getGlobalBounds().height - projectileSize.y) / 2.f;
   projectile->setPosition(startPosition.x + offsetX, startPosition.y + offsetY);
   map.addActor(std::move(projectile));  
}



void Player::sendFirstSpell(Map_old& map, const sf::RenderWindow& window) {
    sf::Vector2f playerPos = this->sprite.getPosition();
	playerPos = sf::Vector2f(playerPos.x + TILE_SIZE / 4.f, playerPos.y + TILE_SIZE / 4.f); //Remplacer les 4.f par 2.f quand le perso sera en 64x64 (c'est fait)

    sf::Vector2i mousePixel = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePixel);

    sf::Vector2f direction = mouseWorld - playerPos;
    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len != 0.f)
        direction /= len;
    else
        direction = sf::Vector2f(1.f, 0.f);

    // Calcul angle
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;

    // Créer le sort directement à la position du joueur
    std::unique_ptr<FirstSpell> projectile = std::make_unique<FirstSpell>(playerPos, direction, this->team);

    // Appliquer la rotation
    projectile->setRotation(angle + 90.f);

    // Ajouter à la map
    map.addActor(std::move(projectile));
}




void Player::update(float dt) {
    if (attackCooldown > 0.f)
        attackCooldown -= dt;
    if (firstSpellCooldown > 0.f)
        firstSpellCooldown -= dt;
    if (secondSpellCooldown > 0.f)
        secondSpellCooldown -= dt;
    if (!vulnerable) {
        vulnerabilityCooldown -= dt;
    }
	if (vulnerabilityCooldown <= 0.f) {
		vulnerable = true; // Le joueur redevient vulnérable
	}
    if (HP <= 0.f) {
        isAlive = false; // Marquer le joueur comme mort
    }


    if (quickenedDurationLeft > 0.f) {
        std::cout << "QUICKENED\n";

        effectiveSpeed = speed * (1.f + quickenedBy);

        quickenedDurationLeft -= dt; // On décrémente la durée restante du boost de vitesse
        if (quickenedDurationLeft <= 0.f) {
            quickenedBy = 0.f; // Le boost de vitesse est terminé
            quickenedDurationLeft = 0.f;
        }
    }
    else {
        effectiveSpeed = speed;
    }


    // Animation :
    animationTimer += dt;
    if (animationTimer >= animationSwitchTime) {
        animationTimer = 0.f;
        useFirstTexture = !useFirstTexture;

        changeTexture(dt);
    }


    

}
