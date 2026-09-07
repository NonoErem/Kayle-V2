#pragma once

#include "actor.hpp"
#include <SFML/System.hpp> // Ajout pour sf::Vector2f
#include <cmath>


class Spell : public Actor {
public:
    Spell();
    Spell(const sf::Vector2f& startPos, const sf::Vector2f& direction, int team);
    void update(float dt) override;
    void setRotation(float angle) { sprite.setRotation(angle); }
    float lifetime = 2.0f;
    float damage = 1.f;
    std::string getId() const override;

protected:
    sf::Vector2f direction;

};



class Projectile : public Spell {
public:
    Projectile();
    Projectile(const sf::Vector2f& startPos, const sf::Vector2f& direction, int team, float damage);
    float lifetime = 2.0f;
};



class MeleeAttack : public Spell {
public:
    MeleeAttack(const sf::Vector2f& startPos, const sf::Vector2f& direction, int team);

};

class FirstSpell : public Spell {
public:
	FirstSpell(const sf::Vector2f& startPos, const sf::Vector2f& direction, int team);
};