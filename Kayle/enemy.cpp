#include "enemy.hpp"
#include "map_old.hpp"
#include "config.hpp"

#include <iostream>
#include "texturemanager.hpp"


int Enemy::nextId = 1;
int MeleeEnemy::nextId = 1;
int RangedEnemy::nextId = 1;


Enemy::Enemy() {
	sprite.setTexture(TextureManager::getTexture("Textures/ennemies/slime.png"));
    speed = 50.0f;
	effectiveSpeed = speed;
    id = "be" + std::to_string(nextId++);
	HP = 100.f; // Points de vie de l'ennemi
    team = 1;
	isAlive = true; // L'ennemi est vivant par défaut
}

void Enemy::update(float dt) {
    // simple déplacement automatique (par exemple vers la droite)
    sprite.move(effectiveSpeed * dt, 0.f);

    if (HP <= 0.f) {
        isAlive = false; // Marquer l'ennemi comme non vivant
    }

    if (slowDurationLeft > 0.f) {
		effectiveSpeed = speed * (1.f - slowedBy);
    }
}

std::string Enemy::getId() const {
    return id;
}

void Enemy::setId(std::string newId) {
    id = newId;
}



//##################################################################
//######################### MELEE ENEMIES ##########################
//##################################################################


MeleeEnemy::MeleeEnemy() {
	sprite.setTexture(TextureManager::getTexture("Textures/ennemies/slime.png"));
    
    speed = 100.0f;
	effectiveSpeed = speed;
    id = "me" + std::to_string(nextId++);
    HP = 3.f;
    damage = 20.f;
}

std::string MeleeEnemy::getId() const {
    return id;
}


void MeleeEnemy::update(float dt) {
    if (player == nullptr) {
        // Sécurité : éviter un crash si le pointeur n'est pas encore initialisé
        return;
    }

    // Obtenir la position actuelle de l'ennemi
    sf::Vector2f enemyPos = sprite.getPosition();
    sf::Vector2f playerPos = player->getPosition();
    


    // Calculer le vecteur direction vers le joueur
    sf::Vector2f dir = playerPos - enemyPos;

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (length != 0.f) {
        dir /= length; // normalisation
    }

    // Déplacement selon la direction normalisée
    sf::Vector2f velocity = dir * effectiveSpeed;
    
    if (aggro = true) sprite.move(velocity * dt);
    else if (length <= 8 * TILE_SIZE) aggro = true;

    if (HP <= 0.f) {
        isAlive = false; // Marquer l'ennemi comme non vivant
    }

    if (cannotMoveFor > 0.f) {
		canMove = 0; // L'ennemi ne peut pas bouger
        cannotMoveFor -= dt; // On décrémente la durée restante du ralentissement
        if (cannotMoveFor <= 0.f) {
            canMove = 1; // Le ralentissement est terminé
            cannotMoveFor = 0.f;
        }
    }
    if (slowDurationLeft > 0.f) {
        slowDurationLeft -= dt; // On décrémente la durée restante du ralentissement
        if (slowDurationLeft <= 0.f) {
            slowedBy = 0.f; // Le ralentissement est terminé
            slowDurationLeft = 0.f;
        }
    }
	effectiveSpeed = speed * (1.f - slowedBy) * canMove;
    
}




//##################################################################
//######################### RANGED ENEMIES #########################
//##################################################################


RangedEnemy::RangedEnemy() {
	sprite.setTexture(TextureManager::getTexture("Textures/ennemies/slime.png"));

    speed = 100.0f;
    id = "re" + std::to_string(nextId++);
    HP = 100.f;
    damage = 10.f;
    fireCooldown = 0.f;       // temps restant avant de pouvoir tirer
    fireRate = 1.f;
    team = 1;
}

std::string RangedEnemy::getId() const {
    return id;
}


void RangedEnemy::sendProjectile(Map_old& map, const sf::RenderWindow& window) {
    
    if (player == nullptr) {
        // Sécurité : éviter un crash si le pointeur n'est pas encore initialisé
        return;
    }


    if (fireCooldown <= 0.f)
    {
        fireCooldown = fireRate;
    }
    else {
        return;
    }

    // Créer un projectile à la position actuelle de l'ennemi
    sf::Vector2f playerPos = player->getPosition();
    sf::Vector2f startPosition = sprite.getPosition();
    sf::Vector2f direction = playerPos - startPosition;

    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0.f)
		direction /= length; // normalisation

    std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(getPosition(), direction, this->team, damage);

    sf::Vector2f projectileSize = projectile->shape.getSize();

    float offsetX = (sprite.getGlobalBounds().width - projectileSize.x) / 2.f;
    float offsetY = (sprite.getGlobalBounds().height - projectileSize.y) / 2.f;

    projectile->setPosition(startPosition.x + offsetX, startPosition.y + offsetY);
    map.addActor(std::move(projectile));
}


void RangedEnemy::update(float dt) {
    if (player == nullptr) {
        // Sécurité : éviter un crash si le pointeur n'est pas encore initialisé
        return;
    }

    // Obtenir la position actuelle de l'ennemi
    sf::Vector2f enemyPos = sprite.getPosition();
    sf::Vector2f playerPos = player->getPosition();



    // Calculer le vecteur direction vers le joueur
    sf::Vector2f dir = playerPos - enemyPos;

    float length = std::sqrt(dir.x * dir.x + dir.y * dir.y);

    if (length != 0.f) {
        dir /= length; // normalisation
    }


    if (fireCooldown > 0.f)
        fireCooldown -= dt;

    if (HP <= 0.f) {
        isAlive = false; // Marquer l'ennemi comme non vivant

        std::cout << id << " mort !" << std::endl;
    }
}