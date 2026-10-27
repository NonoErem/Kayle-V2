#pragma once
#include <SFML/Graphics.hpp>
#include "unique_entity.hpp"
class Entity
{
public:
	Entity(Unique_entity& uniqueEntityHere);
	Unique_entity uniqueEntity;
	void update(float dt);

	int posX = 0;
	int posY = 0;
	int HP = 100;
};

