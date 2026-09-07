#include "texturemanager.hpp"
#include <iostream>

std::unordered_map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return it->second;
    }

    // Sinon, on ne la charge
    sf::Texture texture;
    if (!texture.loadFromFile(filename)) {
        std::cerr << "Erreur de chargement texture : " << filename << std::endl;
    }

    // Insertion dans la map
    textures[filename] = std::move(texture);
    return textures[filename];
}
