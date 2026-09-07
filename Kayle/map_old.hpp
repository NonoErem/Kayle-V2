#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <list> // Ajout de l'inclusion correcte pour std::list
#include "enemy.hpp"
#include "player.hpp"
#include "actor.hpp"
#include "tileset.hpp"
using namespace std;




class Map_old {
public:
  Map_old();
  Map_old(const std::string& filename); // nouveau constructeur

  bool loadFromFile(const std::string& filename);
  void saveToFile(const std::string& filename); // nouveau
  void setTile(int x, int y, int type, int layer);
  void setActor(int x, int y, int type); // pour ajouter un ennemi ou un acteur
  void resetMap();
  void resizeMap(int newWidth, int newHeight, int offsetX, int offsetY);
  void ensureTileExists(int x, int y);
  bool isSolidTile(int tileX, int tileY) const;
  bool blockLight(int tileX, int tileY) const;
  bool sameLayerDown(int tileX, int tileY) const;
  bool sameTypeDown(int tileX, int tileY) const;
  void removeActorAt(const sf::Vector2f& pos, float radius);
  int MAP_WIDTH = 1;
  int MAP_HEIGHT = 1;
  int originX = 0;
  int originY = 0;
  TileSet tileSet;
  
  // Ajout de la méthode addActor dans la classe Map  
  void addActor(std::unique_ptr<Actor> actor) {  
      actors.push_back(std::move(actor));  
  }

  void addProjectile(std::unique_ptr<Projectile> proj) {
      projectiles.push_back(std::move(proj));
  }

  std::vector<std::unique_ptr<Actor>>& getActors() {
      return actors;
  }

  const std::vector<std::unique_ptr<Projectile>>& getProjectiles() const {
      return projectiles;
  }

  Player* getPlayer() const {
      return player;
  }

  sf::Vector2i getTilesSize() const {
      if (tiles.empty()) return { 0, 0 };
      int sizeY = static_cast<int>(tiles.size());
      int sizeX = static_cast<int>(tiles[0].size());
      return { sizeX, sizeY };
  }

  int getTileId(int x, int y, int layer) const {
	  if (y < 0 || y >= tiles.size() || x < 0 || x >= tiles[y].size() || layer < 0 || layer >= 5)
		  return -1; // coordonnées invalides
	  return tiles[y][x][layer];
  }



  void drawLayer(sf::RenderTarget& target, int layer, sf::Vector2f, sf::Color color = sf::Color::White);

private:
	std::vector<std::vector<std::vector<int>>> tiles; //positionX, positionY, layer -> tileId
    std::vector < std::unique_ptr<Actor>> actors;
    std::vector < std::unique_ptr<Projectile>> projectiles;
    Player* player = nullptr;

    int width = 0;
    int height = 0;

    sf::Texture grassTexture;
    sf::Sprite grassSprite;
};
