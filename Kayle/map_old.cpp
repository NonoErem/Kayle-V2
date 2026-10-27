#include "map_old.hpp"
#include <fstream>
#include <iostream>
#include <cctype>
#include "config.hpp"
#include <sstream>
#include <memory>
#include "enemy.hpp"
#include "player_old.hpp"
#include "textures.hpp"
#include "tileset.hpp"
#include <SFML/Graphics.hpp>






Map_old::Map_old() {
    tiles.resize(MAP_HEIGHT, std::vector<std::vector<int>>(MAP_WIDTH, std::vector<int>(5, 0)));


    tileSet.loadTile(0, "Textures/mini_tileset.png", sf::IntRect(0, 32, 32, 32)); // grass
    tileSet.loadTile(5, "Textures/mini_tileset.png", sf::IntRect(0, 0, 32, 32)); // wall


    //tileSet.loadTile(0, "Textures/terrain/grass.png");
    tileSet.loadTile(2, "Textures/terrain/roof.png");
    tileSet.loadTile(1, "Textures/terrain/wall.png");
    tileSet.loadTile(3, "Textures/terrain/sand.png");
    tileSet.loadTile(4, "Textures/terrain/water.png");
    //tileSet.loadTile(5, "Textures/terrain/marble_wall.png");
    tileSet.loadTile(6, "Textures/terrain/dirt.png");
    tileSet.loadTile(7, "Textures/terrain/tree.png");
	tileSet.loadTile(8, "Textures/terrain/tree2.png");
	tileSet.loadTile(9, "Textures/terrain/tree3.png");

}



Map_old::Map_old(const std::string& filename) {

    tiles.resize(MAP_HEIGHT, std::vector<std::vector<int>>(MAP_WIDTH, std::vector<int>(5, 0)));
    loadFromFile(filename);


    tileSet.loadTile(0, "Textures/terrain/grass.png");
    tileSet.loadTile(1, "Textures/terrain/water.png");
    tileSet.loadTile(2, "Textures/terrain/stone.png");
    tileSet.loadTile(3, "Textures/terrain/sand.png");
}



//##################################################################
//####################### LOADING OF THE MAP #######################
//##################################################################

bool Map_old::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Erreur : impossible d'ouvrir " << filename << std::endl;
        return false;
    }

    actors.clear();

    std::string line;
    int y = 0;
    int map_width = 5, map_height = 5;


    while (getline(file, line)){
        std::istringstream iss(line);
        std::string token;
        if (iss >> token && token == "##") {
            std::string keyword;
            iss >> keyword;
            if (keyword == "MAP_WIDTH") {
                if (std::getline(file, line)) map_width = std::stoi(line);
                std::cout << "map_width : " << map_width << "\n";

            }
            else if (keyword == "MAP_HEIGHT") {
                if (std::getline(file, line)) map_height = std::stoi(line);
                std::cout << "map_height : " << map_height << "\n";

                std::cout << "map_width : " << map_width << "\n";
                resizeMap(map_width, map_height, 0, 0);

                std::cout << "Après le resize \n";
                std::cout << "MAP_WIDTH : " << MAP_WIDTH << "\n";
                std::cout << "MAP_HEIGHT : " << MAP_HEIGHT << "\n";



                auto size = getTilesSize();
                std::cout << "Tiles chargées : " << size.x << " x " << size.y << "\n";

            }
            else if (keyword == "Tiles") {
				std::cout << "TILES\n";

                y = 0;

                

                while (std::getline(file, line) && y < MAP_HEIGHT) {
                    std::istringstream rowIss(line);
                    std::string block;
                    int x = 0;
                    
					while (std::getline(rowIss, block, ',') && x < MAP_WIDTH) {

                        //std::cout << "X = " << x << "\n";

						std::istringstream blockStream(block);
						int v1, v2, v3, v4, v5;
						if (blockStream >> v1 >> v2 >> v3 >> v4 >> v5) {
                            if (x < MAP_WIDTH) {
                                if (x != MAP_WIDTH)
                                {
                                    //std::cerr << "Erreur : largeur incorrecte ligne " << y << std::endl;
                                }                                
                                tiles[y][x][0] = v1;
                                tiles[y][x][1] = v2;
                                tiles[y][x][2] = v3;
                                tiles[y][x][3] = v4;
                                tiles[y][x][4] = v5;

                                ++x;
                            }
                        }
                        //std::cout << "Y = " << y << "\n";
					}

					++y;
				}
            
            }
            else if (keyword == "Actors") {
                std::cout << "ACTORS\n";
                while (std::getline(file, line)) {
                    if (line.empty()) continue;

                    std::istringstream actorIss(line);
                    std::string actorId;
                    int ax, ay;
                    if (actorIss >> actorId >> ax >> ay) {
                        std::unique_ptr<Actor> actor; // Define the enemy variable
                        std::string actorType = actorId.substr(0, 2); // Extract the first two characters of enemyId to determine the type.
                        std::cerr << actorType << std::endl;


                        if (actorType == "me") { // MeleeEnemy
                            actor = std::make_unique<MeleeEnemy>();
                        }
                        else if (actorType == "re") { // RangedEnemy
                            actor = std::make_unique<RangedEnemy>();
                        }
                        else if (actorType == "pl") { // player
                            actor = std::make_unique<Player_old>();
                            player = static_cast<Player_old*>(actor.get());
                        }
                        else {
                            std::cerr << "Type d'acteur inconnu : " << actorType << std::endl;
                            return false; // Exit if the type is invalid
                        }

                        actor->setId(actorId);
                        actor->setPosition(
                            static_cast<float>(ax * TILE_SIZE * VIEW_SCALE),
                            static_cast<float>(ay * TILE_SIZE * VIEW_SCALE)
                        );
                        actors.push_back(std::move(actor));
                        std::cout << "Acteur de type " << actorType << " trouvé à la position(" << ax << ", " << ay << ")\n";
                        // gérer d'autres types
                    }
                }
            }
        }
    }


 

    Player_old* playerPtr = nullptr;

    // On cherche le joueur dans la liste
    for (const auto& actor : actors) {
        if (auto p = dynamic_cast<Player_old*>(actor.get())) {
            playerPtr = p;
            break;
        }
    }

    // On donne le pointeur player à tous les ennemis
    if (playerPtr != nullptr) {
        for (const auto& actor : actors) {
            if (auto enemy = dynamic_cast<Enemy*>(actor.get())) {
                enemy->setPlayer(playerPtr);
            }
        }
    }

    return true;
}






//##################################################################
//######################### MAP RENDERING ##########################
//##################################################################


void Map_old::drawLayer(sf::RenderTarget& target, int layer, sf::Vector2f playerPos, sf::Color color) {
    for (int y = std::max(-4 + (playerPos.y / VIEW_SCALE - 1 - SCREEN_HEIGHT / 2) / TILE_SIZE, 0.f); y < std::min(std::max(2 + (playerPos.y / VIEW_SCALE  + SCREEN_HEIGHT / 2) / TILE_SIZE, (float)SCREEN_HEIGHT / TILE_SIZE),(float)tiles.size()); ++y) {
        for (int x = std::max(-4 + (playerPos.x / VIEW_SCALE - SCREEN_WIDTH / 2) / TILE_SIZE, 0.f); x < std::min(std::max(2 + (playerPos.x / VIEW_SCALE + SCREEN_WIDTH / 2) / TILE_SIZE, (float)SCREEN_WIDTH / TILE_SIZE), (float)tiles[y].size()); ++x) {
            int tileId = tiles[y][x][layer];
            if (tileId < 0) continue;

            const sf::Sprite* sprite = tileSet.getTileSprite(tileId);
            float worldX = (x - originX) * TILE_SIZE * VIEW_SCALE;
            float worldY = (y - originY) * TILE_SIZE * VIEW_SCALE;



            if (sprite) {
                sf::Sprite s = *sprite;
                s.setOrigin(s.getLocalBounds().width / 2.f, s.getLocalBounds().height);
                s.setPosition(worldX + (TILE_SIZE * VIEW_SCALE) / 2.f, worldY + TILE_SIZE * VIEW_SCALE);
//                s.setPosition(worldX, worldY);
                s.setColor(color);
                target.draw(s);
            }
            else {
                sf::RectangleShape fallback(sf::Vector2f(TILE_SIZE * VIEW_SCALE, TILE_SIZE * VIEW_SCALE));
                fallback.setFillColor(sf::Color(100, 100, 100));
                fallback.setPosition(worldX, worldY);
                target.draw(fallback);
            }
        }
    }
}




//##################################################################
//########################## MAP EDITING ###########################
//##################################################################


void Map_old::resetMap() {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            tiles[y][x][0] = 0;
            tiles[y][x][1] = -1;
            tiles[y][x][2] = -1;
            tiles[y][x][3] = -1;
            tiles[y][x][4] = -1;

        }
    }
}
void Map_old::resizeMap(int newWidth, int newHeight, int offsetX, int offsetY) {
    // Pour chaque ligne (axe Y)
    std::cout << "new_width : " << newWidth << "\n";

    if (offsetY > 0) {
        // On ajoute des lignes au-dessus
        tiles.insert(tiles.begin(), offsetY, std::vector<std::vector<int>>(MAP_WIDTH, std::vector<int>(5, -1)));
        MAP_HEIGHT += offsetY;
        originY += offsetY;
    }
    else if (newHeight > MAP_HEIGHT) {
        std::cout << "BOUCLE map_height \n";

        // On ajoute des lignes en bas
        tiles.resize(newHeight, std::vector<std::vector<int>>(MAP_WIDTH, std::vector<int>(5, -1)));
        MAP_HEIGHT = newHeight;
    }

    // Pour chaque colonne (axe X)
    for (auto& row : tiles) {
        std::cout << "MAP_WIDTH : " << MAP_WIDTH << "\n";
        std::cout << "new_width : " << newWidth << "\n";


        if (offsetX > 0) {
            // Ajouter des colonnes à gauche
            row.insert(row.begin(), offsetX, std::vector<int>(5, -1));
        }
        else if (newWidth > MAP_WIDTH) {
            // Ajouter des colonnes à droite
            std::cout << "BOUCLE map_width \n";

            row.resize(newWidth, std::vector<int>(5, -1));
        }
    }

    MAP_WIDTH = newWidth + offsetX;
    originX += offsetX;
}


void Map_old::setTile(int x, int y, int type, int layer) {
    ensureTileExists(x, y);

    int ix = x + originX;
    int iy = y + originY;

    if (ix < 0 || iy < 0 || ix >= MAP_WIDTH || iy >= MAP_HEIGHT)
        return;

    if (type >= 0 && type < 9) {
        tiles[iy][ix][layer] = type;
    }
    else if (type == 9) {
        tiles[iy][ix][0] = -1;
        tiles[iy][ix][1] = -1;
        tiles[iy][ix][2] = -1;
        tiles[iy][ix][3] = -1;
        tiles[iy][ix][4] = -1;


    }
}

void Map_old::ensureTileExists(int x, int y) {
    int ix = x + originX;
    int iy = y + originY;

    int offsetX = 0, offsetY = 0;
    int newWidth = MAP_WIDTH;
    int newHeight = MAP_HEIGHT;

    // S'étendre à gauche
    if (ix < 0) {
        offsetX = -ix;
    }
    // S'étendre à droite
    else if (ix >= MAP_WIDTH) {
        newWidth = ix + 1;
    }

    // S'étendre vers le haut
    if (iy < 0) {
        offsetY = -iy;
    }
    // S'étendre vers le bas
    else if (iy >= MAP_HEIGHT) {
        newHeight = iy + 1;
    }

    if (offsetX > 0 || offsetY > 0 || newWidth > MAP_WIDTH || newHeight > MAP_HEIGHT) {
        resizeMap(newWidth, newHeight, offsetX, offsetY);
    }
}


void Map_old::setActor(int x, int y, int type) {
    ensureTileExists(x, y);

    int realX = x + originX;
    int realY = y + originY;

    std::unique_ptr<Actor> actor;

    switch (type) {
    case 0:
        actor = std::make_unique<Player_old>();
        break;
    case 1:
        actor = std::make_unique<MeleeEnemy>();
        break;
    case 2:
        actor = std::make_unique<RangedEnemy>();
        break;
    default:
        std::cerr << "Type d'acteur inconnu : " << type << std::endl;
        return;
    }

    actor->setPosition(static_cast<float>(x * TILE_SIZE * VIEW_SCALE),
        static_cast<float>(y * TILE_SIZE * VIEW_SCALE));
    actors.push_back(std::move(actor));

    std::cout << "Ajout d'un acteur " << type << " à (" << x << ", " << y << ")\n";
}





bool Map_old::isSolidTile(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT)
        return true; // hors de la map = solide

    std::map<int, TileInfo> tileInfo = texturesInfo();

    for (int layer = 0; layer < 5; ++layer) {
        if (tileInfo[tiles[tileY][tileX][layer]].isSolid)
            return true;
    }
    return false;
}



bool Map_old::blockLight(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY >= MAP_HEIGHT)
        return true; // hors de la map = solide

    std::map<int, TileInfo> tileInfo = texturesInfo();

    for (int layer = 0; layer < 5; ++layer) {
        if (tileInfo[tiles[tileY][tileX][layer]].blocksLight)
            return true;
    }
    return false;

}


bool Map_old::sameLayerDown(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY+1 >= MAP_HEIGHT)
        return true; // hors de la map = solide

    std::map<int, TileInfo> tileInfo = texturesInfo();

    for (int layer = 0; layer < 5; ++layer) {
        if (tileInfo[tiles[tileY][tileX][layer]].blocksLight && tileInfo[tiles[tileY+1][tileX][layer]].blocksLight)
            return true;
    }
    return false;

}

bool Map_old::sameTypeDown(int tileX, int tileY) const {
    if (tileX < 0 || tileX >= MAP_WIDTH || tileY < 0 || tileY + 1 >= MAP_HEIGHT)
        return true; // hors de la map = solide

    std::map<int, TileInfo> tileInfo = texturesInfo();

    for (int layer1 = 0; layer1 < 5; ++layer1) {
        for (int layer2 = 0; layer2 < 5; ++layer2) {
            if (layer1 == layer2) continue; // déjà géré par sameLayerDown
            int id = tileInfo[tiles[tileY][tileX][layer1]].tileId;
            if (id !=0 && id !=-1 && id == tileInfo[tiles[tileY + 1][tileX][layer2]].tileId)
                return true;
        }
    }

    return false;

}


void Map_old::removeActorAt(const sf::Vector2f& pos, float radius)
{
    auto it = std::remove_if(actors.begin(), actors.end(),
        [&](const std::unique_ptr<Actor>& actor) {
            sf::Vector2f actorPos = actor->getPosition();
            float dist = std::hypot(pos.x - actorPos.x, pos.y - actorPos.y);
            return dist <= radius;
        });

    if (it != actors.end()) {
        actors.erase(it, actors.end());
    }
}





//##################################################################
//####################### SAVING OF THE MAP ########################
//##################################################################


void Map_old::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Erreur : impossible d'ouvrir " << filename << " en écriture.\n";
        return;
    }


	file << "## MAP_WIDTH ##\n" << MAP_WIDTH << "\n";
    file << "## MAP_HEIGHT ##\n" << MAP_HEIGHT << "\n";
    file << "## Tiles ##\n";

    // Enregistrer les tiles
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            file << tiles[y][x][0] << " " << tiles[y][x][1] << " " << tiles[y][x][2] << " " << tiles[y][x][3] << " " << tiles[y][x][4];
            if (x < MAP_WIDTH - 1) file << ",";
        }
        file << "\n";
    }

    file << '\n';

	// Enregistrer les acteurs

    file << "## Actors ##\n";
    for (const auto& actor : actors) {
        file << actor->getId() << ' ' << actor->getPosition().x / (TILE_SIZE * VIEW_SCALE) << ' ' << actor->getPosition().y / (TILE_SIZE * VIEW_SCALE) << '\n';
    }


    std::cout << "Carte sauvegardée dans " << filename << "\n";
}
