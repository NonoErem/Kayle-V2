#include "effect.hpp"
#include <iostream>

Effect::Effect(const Unique_effect& uniqueEffectHere)
    : uniqueEffect(uniqueEffectHere)
{
    // Récupérer la texture associée au sprite d'origine.
    const sf::Texture* texture = uniqueEffect.sprite.getTexture();

    if (texture != nullptr)
    {
        uniqueEffect.sprite.setTexture(*texture);
    }

    // Initialiser la première frame.
    duration = uniqueEffect.frameDuration * uniqueEffect.frameCount;
    currentFrame = 0;
    frameTimer = 0.f;
    animationFinished = false;

    if (uniqueEffect.frameWidth > 0 &&
        uniqueEffect.frameHeight > 0)
    {
        uniqueEffect.sprite.setTextureRect(
            sf::IntRect(
                0,
                0,
                uniqueEffect.frameWidth,
                uniqueEffect.frameHeight
            )
        );
    }
}

void Effect::update(float dt)
{
    // Durée de vie totale de l'effet.
    elapsedTimer += dt;

    const int frameWidth = uniqueEffect.frameWidth;
    const int frameHeight = uniqueEffect.frameHeight;
    const int frameCount = uniqueEffect.frameCount;
    const float frameDuration = uniqueEffect.frameDuration;

    // Pas d'animation si les paramètres sont invalides.
    if (frameWidth <= 0 || frameHeight <= 0 || frameCount <= 1 || frameDuration <= 0.f || animationFinished)
    {
        return;
    }

    frameTimer += dt;

    while (frameTimer >= frameDuration)
    {
        frameTimer -= frameDuration;

        if (currentFrame + 1 < frameCount)
        {
            ++currentFrame;
        }
        else if (uniqueEffect.looping)
        {
            currentFrame = 0;
        }
        else
        {
            animationFinished = true;
            return;
        }

        // Toutes les frames sont disposées horizontalement.
        uniqueEffect.sprite.setTextureRect(
            sf::IntRect(
                currentFrame * frameWidth,
                0,
                frameWidth,
                frameHeight
            )
        );
    }
}