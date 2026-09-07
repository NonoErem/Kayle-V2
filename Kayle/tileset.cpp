#include "tileset.hpp"
#include "config.hpp"
#include <iostream>

bool TileSet::loadTile(int tileId, const std::string& texturePath, sf::IntRect rect) {
    sf::Texture texture;

    bool useRect = (rect.width > 0 && rect.height > 0);

    if (useRect) {
        if (!texture.loadFromFile(texturePath, rect)) {
            std::cerr << "Erreur : impossible de charger la texture " << texturePath << "\n";
            return false;
        }
    }
    else {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Erreur : impossible de charger la texture " << texturePath << "\n";
            return false;
        }
    }

    // Copie locale dans la map (nécessaire car sf::Texture ne se copie pas automatiquement)
    textures[tileId] = std::move(texture);

    sf::Sprite sprite;
    sprite.setTexture(textures[tileId]);
    sprite.setScale(VIEW_SCALE, VIEW_SCALE);

    sprites[tileId] = sprite;
    return true;
}


const sf::Sprite* TileSet::getTileSprite(int tileId) const {
    auto it = sprites.find(tileId);
    if (it != sprites.end())
        return &it->second;
    return nullptr;
}

