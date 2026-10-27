#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Unique_effect {
public:
    Unique_effect() = default;

    sf::Sprite sprite;

    int id = -1;
    std::string filePath;
    int defaultLayer = 0;
    std::string name = "Effect";

    bool blockEntity = false;
    bool blockLight = false;

    // Configuration de la spritesheet
    int frameWidth = 0;
    int frameHeight = 0;
    int frameCount = 1;
    float frameDuration = 0.05f;
    bool looping = false;
};