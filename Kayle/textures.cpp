#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "textures.hpp"


enum TileType {
    GRASS = 0,
    WALL = 1,
    ROOF = 2,
    SAND = 3,
	WATER = 4,
	MARBLE_WALL = 5,
	DIRT = 6,
    TREE = 7,
	TREE2 = 8,
	TREE3 = 9

};



std::map<int, TileInfo> texturesInfo() { // des infos sur les tiles
    std::map<int, TileInfo> tilePaths;


    tilePaths[GRASS] = { GRASS, "Textures/terrain/grass.png" , 0, false, false };
    tilePaths[WALL] = { WALL,  "Textures/terrain/wall.png"  , 2, true, true };
    tilePaths[ROOF] = { ROOF,  "Textures/terrain/roof.png"  , 4, false, false };
    tilePaths[SAND] = { SAND,  "Textures/terrain/sand.png"  , 0, false, false };
    tilePaths[WATER] = { WATER, "Textures/terrain/water.png" , 0, true, false };
    tilePaths[MARBLE_WALL] = { MARBLE_WALL, "Textures/terrain/marble_wall.png"  , 2, true, true };
    tilePaths[DIRT] = { DIRT, "Textures/terrain/dirt.png" , 4, false, false };
    tilePaths[TREE] = { TREE, "Textures/terrain/tree.png" , 2, true, true };
    tilePaths[TREE2] = { TREE, "Textures/terrain/tree2.png" , 3, false, true };
    tilePaths[TREE3] = { TREE3, "Textures/terrain/tree3.png" , 2, false, false };


    return tilePaths;
}

std::map<int, ActorInfo> actorsInfo() {
    std::map<int, ActorInfo> actorPaths;


    actorPaths[0] = { "pl", "Textures/player/front_player_A.png" };
    actorPaths[1] = { "me", "Textures/ennemies/slime.png" };
    actorPaths[2] = { "re", "Textures/ennemies/slime.png" };


    return actorPaths;
}