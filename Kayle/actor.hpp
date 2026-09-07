// actor.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
class Map_old;               // forward


class Actor {
public:
    Actor();

    virtual ~Actor() = default;

    virtual void handleInput(float dt);
    virtual void update(float dt);
    virtual void updateWithPlayer(float dt, const sf::Vector2f& playerPos);
    virtual void draw(sf::RenderWindow& window);
    virtual std::string getId() const = 0;
    virtual void setId(std::string newId);
    virtual void sendProjectile(Map_old& map, const sf::RenderWindow& window);

    
    bool hasCollisions = true; // Indique si l'acteur est sujet aux collisions
    virtual void setPosition(float x, float y);

    bool isAlive = true;
	float maxHP = 100.f; // Points de vie maximum de l'acteur
	float HP = 100.f; // Points de vie de l'acteur
    float speed = 0.f;
    float effectiveSpeed = speed;
	float cannotMoveFor = 0.f; // Temps restant pendant lequel l'acteur ne peut pas bouger
    bool canMove = 1;
	float damage = 0.f; // Dégâts infligés par l'acteur
    bool vulnerable = true;
	float vulnerabilityCooldown = 0.f; // Temps restant avant de redevenir vulnérable
	float slowedBy = 0.f; // Pourcentage de slow
	float slowDurationLeft = 0.f; // Durée restante du ralentissement
    int team = 1;
    sf::Vector2f getPosition() const;
    sf::FloatRect getHitBox() const;
    virtual void move(sf::Vector2f movement);
    sf::RectangleShape shape;

private:
	

protected:
    std::string id;
    sf::Texture texture; // Texture de l'acteur
    sf::Sprite sprite;
};




//##################################################################
//####################### ENTITY DEFINITION ########################
//##################################################################


class Entity : public Actor {
public:
    Entity();
};