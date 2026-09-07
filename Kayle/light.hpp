#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map_old.hpp"

// Un segment qui bloque la lumière
struct Edge {
    sf::Vector2f a, b;
    int type;
	int numberOfTilesDown;
};

std::vector<Edge> generateEdgesFromMap(const Map_old& map);

sf::Vector2f normalize(const sf::Vector2f& v);

// Une source de lumière
// light.hppd
class Light {
public:
    enum class Type {
        Circle,   // Lumière dégradée circulaire (actuel)
        Texture   // Image projetée
    };

    Light(const sf::Vector2f& pos, float radius = 400.f, Type type = Type::Circle);

    void setPosition(const sf::Vector2f& pos);
    void setRadius(float r);
    void setTexture(const sf::Texture* tex); // assigner une image
    void setType(Type t);

    void render(sf::RenderTexture& lightMap, const std::vector<Edge>& edges);

private:
    sf::Vector2f position;
    float radius;
    Type type;
    const sf::Texture* texture = nullptr;

    std::vector<sf::Vector2f> castRays(const std::vector<Edge>& edges);
};


// Gère toutes les lumières et la lightmap
class LightSystem {
public:
    LightSystem(unsigned int width, unsigned int height);

    void addLight(const Light& light);
    void clearLights();

    // Dessiner la lightmap sur la fenêtre
    void render(sf::RenderWindow& window, const std::vector<Edge>& edges);

private:
    sf::RenderTexture lightMap;
    std::vector<Light> lights;
};
