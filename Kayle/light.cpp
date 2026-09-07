#include "light.hpp"
#include <algorithm>
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "config.hpp"
#include "map_old.hpp"
#include <execution>


std::vector<Edge> generateEdgesFromMap(const Map_old& map) {
    std::vector<Edge> edges;

    for (int y = 0; y < map.MAP_HEIGHT; y++) {
        for (int x = 0; x < map.MAP_WIDTH; x++) {
            if (!map.blockLight(x, y)) continue;

            float left = x * TILE_SIZE * VIEW_SCALE;
            float right = (x + 1) * TILE_SIZE * VIEW_SCALE;

            const sf::Sprite* sprite = map.tileSet.getTileSprite(map.getTileId(x, y, 2));

            if (sprite) {
                sf::Sprite s = *sprite;
                left = (x * TILE_SIZE + (TILE_SIZE - s.getLocalBounds().width) / 2) * VIEW_SCALE;
                right = ((x + 1) * TILE_SIZE - (TILE_SIZE - s.getLocalBounds().width) / 2) * VIEW_SCALE;
                cout << "Tile (" << x << ", " << y << ") has sprite width: " << left-right << "\n";

            }
            float top = y * TILE_SIZE * VIEW_SCALE;
            float bottom = (y + 1) * TILE_SIZE * VIEW_SCALE;


            bool tileDown = false;
			int numberOfTilesDown = 0;

			int checkY = y;
            while (map.blockLight(x, checkY + 1) && (map.sameLayerDown(x, checkY) || map.sameTypeDown(x, checkY))) {
				numberOfTilesDown++;
                checkY++;
            }

            
            // Bord haut
            //edges.push_back({ sf::Vector2f(left, top), sf::Vector2f(right, top), 3 });
            if (!map.blockLight(x, y - 1)) {
                // Bord haut
                edges.push_back({ sf::Vector2f(left, top), sf::Vector2f(right, top), 3, numberOfTilesDown });
            }


            if (!map.blockLight(x + 1, y + numberOfTilesDown) || !map.blockLight(x + 1, y)) { //anciennement if (!map.blockLight(x + 1, y + numberOfTilesDown) || !map.blockLight(x + 1, y)) {
                // Bord droit
                edges.push_back({ sf::Vector2f(right, top), sf::Vector2f(right, bottom), 2, numberOfTilesDown });
            }

            if (!map.blockLight(x - 1, y + numberOfTilesDown) || !map.blockLight(x - 1, y)) { // anciennement if (!map.blockLight(x - 1, y + numberOfTilesDown) || !map.blockLight(x - 1, y)) {
                // Bord gauche
                edges.push_back({ sf::Vector2f(left, top), sf::Vector2f(left, bottom), 1, numberOfTilesDown });
            }
            

            // Bord bas
            //edges.push_back({ sf::Vector2f(left, bottom), sf::Vector2f(right, bottom) });
            // Bord gauche
//            if (std::find(edges.begin(), edges.end(), x - 1) != edges.end()) {
            //edges.push_back({ sf::Vector2f(left, top), sf::Vector2f(left, bottom), 1 });
            // Bord droit
            //edges.push_back({ sf::Vector2f(right, top), sf::Vector2f(right, bottom), 2 });
        }
    }

    return edges;
}

//==============================
// Light
//==============================
sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len == 0.f) return sf::Vector2f(0.f, 0.f);
    return sf::Vector2f(v.x / len, v.y / len);
}


Light::Light(const sf::Vector2f& pos, float r, Type t)
    : position(pos), radius(r), type(t) {}

void Light::setPosition(const sf::Vector2f& pos) {
    position = pos;
}

void Light::setRadius(float r) {
    radius = r;
}

void Light::setTexture(const sf::Texture* tex) {
    texture = tex;
}

void Light::setType(Type t) {
    type = t;
}

std::vector<sf::Vector2f> Light::castRays(const std::vector<Edge>& edges) {
    const float EPSILON = 1e-6f;
    const int NUM_RAYS = 32;

    std::vector<sf::Vector2f> points;
    points.reserve(NUM_RAYS + edges.size() * 6); // on réserve pour éviter les reallocs

    //===============================
    // 1. Rayons circulaires réguliers
    //===============================
    std::vector<sf::Vector2f> uniformRays(NUM_RAYS);
    std::for_each(std::execution::par, uniformRays.begin(), uniformRays.end(),
        [&](sf::Vector2f& dst) {
            int i = &dst - &uniformRays[0]; // index local
            float angle = (2 * M_PI * i) / NUM_RAYS;

            sf::Vector2f rayDirection = normalize({ std::cos(angle), std::sin(angle) });
            sf::Vector2f closest = position + rayDirection * radius;
            float closestDist = radius;

            for (const auto& edge : edges) {
                sf::Vector2f pos = position;
                sf::Vector2f rayDir = rayDirection;
                sf::Vector2f q = edge.a;
                sf::Vector2f s = edge.b - edge.a;

                float rxs = rayDir.x * s.y - rayDir.y * s.x;
                float q_pxr = (q.x - pos.x) * rayDir.y - (q.y - pos.y) * rayDir.x;

                if (std::fabs(rxs) < EPSILON) continue;

                float t = ((q.x - pos.x) * s.y - (q.y - pos.y) * s.x) / rxs;
                float u = q_pxr / rxs;

                if (t >= 0 && u >= 0 && u <= 1) {
                    sf::Vector2f intersection = pos + t * rayDir;
                    float dist = std::hypot(intersection.x - position.x, intersection.y - position.y);

                    int quadX = intersection.x / (TILE_SIZE * VIEW_SCALE);
                    int quadY = intersection.y / (TILE_SIZE * VIEW_SCALE);
                    int tilePositionX = position.x / (TILE_SIZE * VIEW_SCALE);
                    int tilePositionY = position.y / (TILE_SIZE * VIEW_SCALE);

                    if (edge.type == 1 && tilePositionX < quadX && tilePositionY > quadY + edge.numberOfTilesDown) {
                        continue;
                    }
                    else if (edge.type == 2 && tilePositionX + 1 > quadX && tilePositionY > quadY + edge.numberOfTilesDown) {
                        continue;
                    }



                    if (dist < closestDist) {
                        closestDist = dist;
                        closest = intersection;
                    }
                }
            }
            dst = closest;
        });

    points.insert(points.end(), uniformRays.begin(), uniformRays.end());

    //================================
    // 2. Rayons vers chaque sommet
    //================================
    std::vector<sf::Vector2f> vertexRays(edges.size() * 6);
    std::for_each(std::execution::par, vertexRays.begin(), vertexRays.end(),[&](sf::Vector2f& dst) {
            int idx = &dst - &vertexRays[0];
            int edgeIdx = idx / 6;
            int variant = idx % 6;

            if (edgeIdx >= (int)edges.size()) return;

            const auto& e = edges[edgeIdx];
            sf::Vector2f vertex = (variant < 3) ? e.a : e.b;
            float delta = (variant % 3 == 0 ? -0.0001f : (variant % 3 == 1 ? 0.f : 0.0001f));

            float angle = std::atan2(vertex.y - position.y, vertex.x - position.x) + delta;
            sf::Vector2f rayDir(std::cos(angle), std::sin(angle));

            sf::Vector2f closest = position + rayDir * radius;
            float closestDist = radius;

            for (const auto& edge : edges) {
                sf::Vector2f p = position;
                sf::Vector2f r = rayDir;
                sf::Vector2f q = edge.a;
                sf::Vector2f s = edge.b - edge.a;

                float rxs = r.x * s.y - r.y * s.x;
                float q_pxr = (q.x - p.x) * r.y - (q.y - p.y) * r.x;

                if (std::fabs(rxs) < EPSILON) continue;

                float t = ((q.x - p.x) * s.y - (q.y - p.y) * s.x) / rxs;
                float u = q_pxr / rxs;

                if (t >= 0 && u >= 0 && u <= 1) {
                    sf::Vector2f intersection = p + t * r;
                    float dist = std::hypot(intersection.x - position.x, intersection.y - position.y);
// EN COURS                    

                    int quadX = intersection.x / (TILE_SIZE * VIEW_SCALE);
                    int quadY = intersection.y / (TILE_SIZE * VIEW_SCALE);
					int tilePositionX = position.x / (TILE_SIZE * VIEW_SCALE);
                    int tilePositionY = position.y / (TILE_SIZE * VIEW_SCALE);
                    if (edge.type == 1 && tilePositionX < quadX && tilePositionY > quadY + edge.numberOfTilesDown) {
						continue;
					}
					else if (edge.type == 2 && tilePositionX+1 > quadX && tilePositionY > quadY + edge.numberOfTilesDown) {
                        continue;
					}
                    
                    if (dist < closestDist) {
                        closestDist = dist;
                        closest = intersection;
                    }
                }
            }
            dst = closest;
        });

    points.insert(points.end(), vertexRays.begin(), vertexRays.end());

    //===============================
    // Tri par angle
    //===============================
    std::sort(points.begin(), points.end(),
        [&](const sf::Vector2f& a, const sf::Vector2f& b) {
            float angA = std::atan2(a.y - position.y, a.x - position.x);
            float angB = std::atan2(b.y - position.y, b.x - position.x);
            return angA < angB;
        });

    return points;
}



void Light::render(sf::RenderTexture& lightMap, const std::vector<Edge>& edges) {
    auto points = castRays(edges);

    sf::VertexArray fan(sf::TriangleFan);

    if (type == Type::Circle) {
        // Lumière classique (dégradé blanc -> transparent)
        fan.append(sf::Vertex(position, sf::Color(255, 255, 255, 255)));
        for (auto& p : points) {
            float distToLight = std::hypot(p.x - position.x, p.y - position.y) / radius;
            float transparence = 255 * (1 - (distToLight));
            fan.append(sf::Vertex(p, sf::Color(255, 255, 255, transparence)));
        }
        if (!points.empty()) {
            float distToLight = std::hypot(points.front().x - position.x, points.front().y - position.y) / radius;
            float transparence = 255 * (1 - (distToLight));
            fan.append(sf::Vertex(points.front(), sf::Color(255, 255, 255, transparence)));
        }
        lightMap.draw(fan, sf::BlendAdd);
    }
    else if (type == Type::Texture && texture) {
        // Lumière image projetée
        fan.append(sf::Vertex(position, sf::Color::White,
            { texture->getSize().x / 2.f, texture->getSize().y / 2.f }));
        for (auto& p : points) {
            sf::Vector2f relative = p - position;
            sf::Vector2f texCoord(
                texture->getSize().x / 2.f + relative.x,
                texture->getSize().y / 2.f + relative.y
            );
            fan.append(sf::Vertex(p, sf::Color::White, texCoord));
        }
        if (!points.empty()) {
            sf::Vector2f relative = points.front() - position;
            sf::Vector2f texCoord(
                texture->getSize().x / 2.f + relative.x,
                texture->getSize().y / 2.f + relative.y
            );
            fan.append(sf::Vertex(points.front(), sf::Color::White, texCoord));
        }
        lightMap.draw(fan, texture); // dessin avec l’image
    }
}



//==============================
// LightSystem
//==============================
LightSystem::LightSystem(unsigned int width, unsigned int height) {
    lightMap.create(width, height);
    lightMap.setSmooth(true);
}

void LightSystem::addLight(const Light& light) {
    lights.push_back(light);
}

void LightSystem::clearLights() {
    lights.clear();
}

void LightSystem::render(sf::RenderWindow& window, const std::vector<Edge>& edges) {
    lightMap.clear(sf::Color(100,100,100)); // Couleur de la lumière ambiante

    for (auto& light : lights) {
        light.render(lightMap, edges);
    }

    lightMap.display();

    sf::Sprite sprite(lightMap.getTexture());
    window.draw(sprite, sf::BlendMultiply);
}
