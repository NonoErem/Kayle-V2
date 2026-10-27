#include "entity.hpp"

Entity::Entity(Unique_entity& uniqueEntityHere) : uniqueEntity(uniqueEntityHere) {
    if (uniqueEntity.sprite.getTexture() != nullptr) {
        uniqueEntity.sprite.setTexture(*uniqueEntity.sprite.getTexture());
    }
}

void Entity::update(float dt)
{
	//posX++;
}
