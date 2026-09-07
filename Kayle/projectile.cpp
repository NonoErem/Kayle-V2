#include "projectile.hpp"
#include "config.hpp"
#include <iostream>
#include "texturemanager.hpp"



Spell::Spell(const sf::Vector2f& startPos, const sf::Vector2f& direction, int teamID) {

    sprite.setTexture(TextureManager::getTexture("Textures/projectile.png"));
    sprite.setScale(0.1f * VIEW_SCALE, 0.1f * VIEW_SCALE);


    speed = 300.f;
    id = "spell";
    team = teamID;

    // Normaliser la direction
    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len != 0.f)
        this->direction = sf::Vector2f(direction.x / len, direction.y / len);
    else
        this->direction = sf::Vector2f(1.f, 0.f); // par défaut vers la droite


    sprite.setPosition(startPos);
}



Projectile::Projectile()
{
}

Projectile::Projectile(const sf::Vector2f& startPos, const sf::Vector2f& direction, int teamID, float projectileDamage) {

   sprite.setTexture(TextureManager::getTexture("Textures/projectile.png"));
   sprite.setScale(0.4f * VIEW_SCALE, 0.4f * VIEW_SCALE);

   
   speed = 500.f;
   id = "projectile";
   team = teamID;
   damage = projectileDamage;


   // Normaliser la direction
   float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
   if (len != 0.f)
       this->direction = sf::Vector2f(direction.x / len, direction.y / len);
   else
       this->direction = sf::Vector2f(1.f, 0.f); // par défaut vers la droite


   sprite.setPosition(startPos);
}

Spell::Spell()
{
}



MeleeAttack::MeleeAttack(const sf::Vector2f& startPos, const sf::Vector2f& direction, int teamID) {

    sprite.setTexture(TextureManager::getTexture("Textures/meleeHitbox.png"));
    sprite.setScale(1.f * VIEW_SCALE, 1.f * VIEW_SCALE);

    lifetime = 0.1f;

    speed = 0.f;
    id = "meleeAttack";
    team = teamID;

    // Normaliser la direction
    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len != 0.f)
        this->direction = sf::Vector2f(direction.x / len, direction.y / len);
    else
        this->direction = sf::Vector2f(1.f, 0.f); // par défaut vers la droite


    sprite.setPosition(startPos);
    std::cout << "LE PROJECTILE EXISTE\n";
}

FirstSpell::FirstSpell(const sf::Vector2f& startPos, const sf::Vector2f& direction, int teamID) {
    sprite.setTexture(TextureManager::getTexture("Textures/firstSpellHitbox.png"));
    sprite.setScale(1.f * VIEW_SCALE, 1.f * VIEW_SCALE);

    lifetime = 0.1f;

    speed = 0.f;
    id = "firstSpell";
    team = teamID;

    // Normaliser la direction
    float len = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (len != 0.f)
        this->direction = sf::Vector2f(direction.x / len, direction.y / len);
    else
        this->direction = sf::Vector2f(1.f, 0.f); // par défaut vers la droite


    sprite.setPosition(startPos);
    std::cout << "LE PROJECTILE EXISTE\n";

    sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setOrigin(bounds.width / 2.f, bounds.height);
}



void Spell::update(float dt) {
    // Calculer le vecteur direction vers le joueur  
    // sf::Vector2f dir = playerPos - enemyPos;  
    sprite.move(direction * speed * dt);
    lifetime -= dt;
    if (lifetime <= 0.f) {
        isAlive = false; // Marquer le projectile comme non vivant
    }
}


std::string Spell::getId() const {
    return id;
}
