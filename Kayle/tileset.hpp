#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class TileSet {
public:
    bool loadTile(int tileId, const std::string& texturePath, sf::IntRect rect = sf::IntRect(0, 0, 0, 0));

    const sf::Sprite* getTileSprite(int tileId) const;

private:
    std::map<int, sf::Texture> textures;
    std::map<int, sf::Sprite> sprites;

};
