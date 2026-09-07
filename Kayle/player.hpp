// player.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

#include "actor.hpp"
#include "projectile.hpp"  // pour la classe Projectile
class Map_old;               // forward


enum Direction {
    IDLE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_MELEE,
	DOWN_MELEE,
	LEFT_MELEE,
	RIGHT_MELEE
};

class Player : public Entity {
public:

    Direction currentDirection = IDLE;

    // Le constructeur ne connaît pas encore la map : on l'injectera après
    Player();

    int getEffectiveSpeed();



    // Pas de logique supplémentaire pour l'instant
    void update(float dt) override;

    // Identifiant pour la sauvegarde
    std::string getId() const override;

    void meleeWeapon(Map_old& map, const sf::RenderWindow& window);

	// Envoi d'un projectile
    void sendProjectile(Map_old& map, const sf::RenderWindow& window) override;

	void sendFirstSpell(Map_old& map, const sf::RenderWindow& window);

    void changeTexture(float dt);

    float attackCooldown = 0.f;       // temps restant avant de pouvoir attaquer
    float fireRate = 0.1;
	float meleeRate = 1.f;

	float meleeAnimationCooldown = 0.f;
    float meleeAnimationDuration = 0.3f; // Durée de l'animation de l'attaque au corps à corps

    float firstSpellCooldown = 0.f;
	float firstSpellRate = 8.f;

    float secondSpellCooldown = 0.f;
    float secondSpellRate = 12.f;

    // Altérations d'état
	float quickenedBy = 0.f; // multiplicateur de vitesse
	float quickenedDurationLeft = 0.f; // Durée restante du ralentissement

    int level = 1;

private:
       sf::Texture textureIdle[2];
       sf::Texture textureUp[2];
       sf::Texture textureDown[2];
       sf::Texture textureLeft[2];
       sf::Texture textureRight[2];
	   sf::Texture textureMeleeUp[2];
	   sf::Texture textureMeleeDown[2];
	   sf::Texture textureMeleeLeft[2];
	   sf::Texture textureMeleeRight[2];

       bool useFirstTexture = true;
       float animationTimer = 0.f;
       float animationSwitchTime = 0.5f; // toutes les 0.5 secondes
       bool animationFrameToggle = false;
};
