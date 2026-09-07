#pragma once

#include "actor.hpp"
#include "player.hpp"




class Enemy : public Entity {
private:
	static int nextId; // Static variable to keep track of the next ID
	// Unique ID for each enemy
public:
    Enemy();

	std::string getId() const override;
	void setId(std::string newId);

    virtual void update(float dt) override;

	virtual void setPlayer(Player* p) {
		player = p;
	}

	virtual void sendProjectile(Map_old& map, const sf::RenderWindow& window) {
		// Do nothing by default
	}

	bool aggro = false;



protected:
	Player* player = nullptr;
};



//##################################################################
//######################### MELEE ENEMIES ##########################
//##################################################################


class MeleeEnemy : public Enemy {
private:
	static int nextId; // Static variable to keep track of the next ID
	// Unique ID for each enemy
public:
	MeleeEnemy();

	std::string getId() const override;


	void update(float dt) override;

};




//##################################################################
//######################## RANGED ENEMIES ##########################
//##################################################################


class RangedEnemy : public Enemy {
private:
	static int nextId; // Static variable to keep track of the next ID
	// Unique ID for each enemy
public:
	RangedEnemy();

	std::string getId() const override;


	void update(float dt) override;

	void sendProjectile(Map_old& map, const sf::RenderWindow& window) override;

	float fireCooldown;       // temps restant avant de pouvoir tirer
	float fireRate;

};