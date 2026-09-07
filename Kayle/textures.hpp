#pragma once  
#include <string>  


struct TileInfo {
    int tileId;
    std::string texturePath; // chemin de la texture
    int layer;              // couche sur laquelle la tuile est dessinée
    bool isSolid; // indique si la tuile est solide (pour les collisions)
    bool blocksLight;
};

struct ActorInfo {
    std::string actorType;
    std::string texturePath; // chemin de la texture
};


std::map<int, TileInfo> texturesInfo(); // des infos sur les tiles

std::map<int, ActorInfo> actorsInfo();