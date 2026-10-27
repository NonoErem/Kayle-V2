#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "globalvariables.hpp"
#include "map.hpp"

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


class Player {
public:
	Player(Map& mapHere);
	void playerUpdate();
	sf::FloatRect getAttackBounds() const;
	void move(float moveX, float moveY);
	void baseAttack(sf::Vector2f mouseWorld);
	void changeTexture();
	void floatingMovement();
	sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); }

	sf::FloatRect hitbox = sf::FloatRect(500, 600, TILE_SIZE, TILE_SIZE);
	sf::Sprite sprite;
	float frameElapsed = 0.f;
	float frameDuration = 1.f;
	int frameIndex = 0;
	float floatingTime = 0.f;

	Direction currentDirection = IDLE;

	float posX = 500;
	float posY = 600;
	int baseHealth = 100;
	int currentHealth = 100;
	int effectiveSpeed = 200;

	float attackCooldown = 1.f;
	float attackCooldownLeft = 0.f;
	float meleeFrameDuration = 0.3f;
	float normalFrameDuration = 1.f;
	float actionDurationLeft = 0.f;
	float meleeDuration = 0.6f;

	Map& map;

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
};

