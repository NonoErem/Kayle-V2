#pragma once
#include <SFML/Graphics.hpp>

class Unique_entity {
public:
    Unique_entity() {};
    sf::Sprite sprite;
    int id = -1;
    std::string filePath;
    int defaultLayer = 0;
    std::string name = "Entity"; // Utile pour identifier l'entité

    bool blockEntity = false;
    bool blockLight = false;
};

