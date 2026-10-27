#pragma once

#include <SFML/Graphics.hpp>
#include "unique_effect.hpp"

class Effect {
public:
    explicit Effect(const Unique_effect& uniqueEffectHere);

    Unique_effect uniqueEffect;

    void update(float dt);

    float duration = 0.2f;
    float elapsedTimer = 0.f;

    int posX = 0;
    int posY = 0;
    int HP = 100;

    // État de l'animation
    int currentFrame = 0;
    float frameTimer = 0.f;
    bool animationFinished = false;
};