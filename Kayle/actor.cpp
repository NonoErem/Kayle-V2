#include "actor.hpp"
#include "config.hpp"
#include "map_old.hpp"

Actor::Actor() {
    sprite.setScale(VIEW_SCALE, VIEW_SCALE);

    speed = 0.f;
}

void Actor::handleInput(float) {}
void Actor::update(float) {
    if (HP <= 0.f) {
        isAlive = false; // Marquer l'acteur comme non vivant
    }
	if (slowDurationLeft > 0.f) {
		slowDurationLeft -= 0.1f; // On décrémente la durée restante du ralentissement
		if (slowDurationLeft <= 0.f) {
			slowedBy = 0.f; // Le ralentissement est terminé
			slowDurationLeft = 0.f;
		}
	}
}
void Actor::updateWithPlayer(float, const sf::Vector2f&) {}
void Actor::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Actor::setId(std::string newId) {
    id = std::move(newId);
}

void Actor::sendProjectile(Map_old& map, const sf::RenderWindow& window) {
}

void Actor::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

sf::Vector2f Actor::getPosition() const {
    return sprite.getPosition();
}

sf::FloatRect Actor::getHitBox() const {
    return sprite.getGlobalBounds();
}

void Actor::move(sf::Vector2f movement) {
    sprite.move(movement);
}




//##################################################################
//####################### ENTITY DEFINITION ########################
//##################################################################

Entity::Entity()
{
}
