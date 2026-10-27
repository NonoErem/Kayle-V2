#include "map.hpp"



Map::Map(Sprites_manager& spritesManager) : spritesManager(spritesManager) {
	tiles.resize(1, std::vector<std::vector<Unique_tile>>(1, std::vector<Unique_tile>(5, Unique_tile())));
}

void Map::loadFromFile(const std::string& filePath) {

	//std::cerr << "map loadFromFile()" << std::endl;


	std::ifstream file(filePath);
	
	if (!file) {
		std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
		return;
	}
	

	std::string line;
	MAP_WIDTH = 1;
	MAP_HEIGHT = 1;


	while (std::getline(file, line))
	{
		if (line == "## MAP_WIDTH ##") {
			std::getline(file, line);
			MAP_WIDTH = std::stoi(line);
			std::cout << "MAP_WIDTH : " << MAP_WIDTH << std::endl;
		}
		if (line == "## MAP_HEIGHT ##") {
			std::getline(file, line);
			MAP_HEIGHT = std::stoi(line);
			std::cout << "MAP_HEIGHT : " << MAP_HEIGHT << std::endl;
		}
		if (line == "## Tiles ##") {
			int y = 0;
			int x = 0;
			tiles.assign(MAP_WIDTH, std::vector<std::vector<Unique_tile>>(MAP_HEIGHT, std::vector<Unique_tile>(5, Unique_tile())));
			//tiles.resize(MAP_HEIGHT, std::vector<std::vector<int>>(MAP_WIDTH, std::vector<int>(5, 0))); //Use it to resize lol

			while (std::getline(file, line) && y < MAP_HEIGHT) {
				std::string block = "";
				std::istringstream lineIss(line); //To manipulate the "line" string
				x = 0;
				while (std::getline(lineIss, block, ',')) { //Block going from , to ,
					std::istringstream blockStream(block); //To manipulate the "block" string
					int v1, v2, v3, v4, v5; //Each element of the block
					if (blockStream >> v1 >> v2 >> v3 >> v4 >> v5) {
						if (x < MAP_WIDTH) {
							std::cerr << "in the load block" << std::endl;
							if (v1 != -1) tiles[x][y][0] = spritesManager.getUniqueTile(v1);
							if (v2 != -1) tiles[x][y][1] = spritesManager.getUniqueTile(v2);
							if (v3 != -1) tiles[x][y][2] = spritesManager.getUniqueTile(v3);
							if (v4 != -1) tiles[x][y][3] = spritesManager.getUniqueTile(v4);
							if (v5 != -1) tiles[x][y][4] = spritesManager.getUniqueTile(v5); //For whatever reason, on the older version I decided to put 5 layers instead of 3. I have to think about the saving format.
							++x;

						}
					}
					std::cout << block << std::endl;
				}
				++y;
			}
		}
		if (line == "## Entities ##") {
			while (std::getline(file, line) && !line.empty()) {
				std::istringstream iss(line);
				int id;
				int posX, posY;
				if (iss >> id >> posX >> posY) {
					if (id != -1) {
						// Récupère le modèle de l'entité depuis le manager via son ID
						Unique_entity uniqEnt = spritesManager.getUniqueEntity(id);
						Entity loadedEntity(uniqEnt);
						loadedEntity.posX = posX;
						loadedEntity.posY = posY;
						entities.push_back(loadedEntity);
					}
				}
			}
		}
	}
}






void Map::saveToFile(const std::string& filePath) {
	std::cout << "saving the map" << std::endl;

	std::cerr << tiles[0][0][0].id << std::endl;

	std::ofstream file(filePath);

	if (!file) {
		std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
		return;
	}

	file << "## MAP_WIDTH ##\n" << MAP_WIDTH << "\n";
	file << "## MAP_HEIGHT ##\n" << MAP_HEIGHT << "\n";
	file << "## Tiles ##\n";


	for (int y = 0; y < MAP_HEIGHT; ++y) {
		for (int x = 0; x < MAP_WIDTH; ++x) {
			std::cout << "First mark" << std::endl;
			std::cerr << tiles[x][y][0].id << std::endl;
			file << tiles[x][y][0].id << " " << tiles[x][y][1].id << " " << tiles[x][y][2].id << " " << tiles[x][y][3].id << " " << tiles[x][y][4].id;
			if (x < MAP_WIDTH - 1) file << ",";
		}
		file << "\n";
	}

	file << '\n';


	file << "## Entities ##\n";
	for (const auto& ent : entities) {
		file << ent.uniqueEntity.id << " " << ent.posX << " " << ent.posY << "\n";

	}


	std::cout << "Carte sauvegardée\n";

}





void Map::drawLayer(sf::RenderWindow& window, int layer) {

	for (size_t x = 0; x < tiles.size(); ++x) // Pontentiellement draw pourrait se faire en partant de la fin pour simplifier le raycasting
	{
		for (size_t y = 0; y < tiles[x].size(); ++y)
		{
			tiles[x][y][layer].sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);

			window.draw(tiles[x][y][layer].sprite);
		}
	}
}



void Map::displayMap(sf::RenderWindow& window, sf::Sprite playerSprite) {

	for (size_t layer = 0; layer < tiles[0][0].size(); ++layer)
	{
		if (layer == 2) {
			window.draw(playerSprite);

			for (size_t i = 0; i < entities.size(); ++i)
			{
				entities[i].uniqueEntity.sprite.setPosition(entities[i].posX, entities[i].posY);

				window.draw(entities[i].uniqueEntity.sprite);
			}

			for (size_t i = 0; i < effects.size(); ++i)
			{
				effects[i].uniqueEffect.sprite.setPosition(effects[i].posX, effects[i].posY);

				window.draw(effects[i].uniqueEffect.sprite);
			}
		
		}

		drawLayer(window, layer);
	}

	

}










void Map::setTile(Unique_tile& uniqueTile, sf::Vector2f mousePos, int layer, int brushSize)
{
	if (layer < 0 || layer >= 5)
		return;
	if (brushSize <= 0)
		return;

	int mousePosInTile_X = static_cast<int>(std::floor(mousePos.x / TILE_SIZE));
	int mousePosInTile_Y = static_cast<int>(std::floor(mousePos.y / TILE_SIZE));


	size_t currentHeight = 0;


	if (mousePosInTile_Y < 0) {
		size_t rowsToAdd = static_cast<size_t>(-mousePosInTile_Y);

		for (auto& column : tiles)
		{
			column.insert(column.begin(), rowsToAdd, std::vector<Unique_tile>(5));
		}


		// Update the position of all entities to account for the added rows
		for (auto& ent : entities) {
			ent.posY += static_cast<int>(rowsToAdd * TILE_SIZE);
		}

		MAP_HEIGHT += rowsToAdd;
		mousePosInTile_Y = 0;
	}
	

	if (!tiles.empty())
		currentHeight = tiles[0].size();

	if (mousePosInTile_X < 0)
	{
		size_t columnsToAdd = static_cast<size_t>(-mousePosInTile_X);
		tiles.insert(tiles.begin(), columnsToAdd, std::vector<std::vector<Unique_tile>>(currentHeight, std::vector<Unique_tile>(5)));

		// Update the position of all entities to account for the added columns
		for (auto& ent : entities) {
			ent.posX += static_cast<int>(columnsToAdd * TILE_SIZE);
		}

		MAP_WIDTH += columnsToAdd;
		mousePosInTile_X = 0;
	}

	if (mousePosInTile_X >= tiles.size()) {
		tiles.resize(mousePosInTile_X + 1, std::vector<std::vector<Unique_tile>>(currentHeight, std::vector<Unique_tile>(5)));
	}
	for (auto& column : tiles)
	{
		if (mousePosInTile_Y >= static_cast<int>(column.size()))
			column.resize(mousePosInTile_Y + 1, std::vector<Unique_tile>(5));
	}

	MAP_WIDTH = std::max(MAP_WIDTH, static_cast<int>(tiles.size()));
	if (!tiles.empty()) MAP_HEIGHT = std::max(MAP_HEIGHT, static_cast<int>(tiles[0].size()));

	int forStart = - std::floor((brushSize-0.1)/ 2);
	int forEnd = std::floor(brushSize / 2);
	for (int i = forStart; i <= forEnd; ++i) {
		for (int j = forStart; j <= forEnd; ++j) {
			if (mousePosInTile_X + i >= 0 && mousePosInTile_X + i < MAP_WIDTH && mousePosInTile_Y + j >= 0 && mousePosInTile_Y + j < MAP_HEIGHT)
			{
				tiles[mousePosInTile_X + i][mousePosInTile_Y + j][layer] = uniqueTile;
			}
		}
	}
	std::cerr << "brush done" << std::endl;

	

	return;
}




void Map::setEntity(Unique_entity& uniqueEntity, sf::Vector2f mousePos, int layer)
{
	if (layer < 0 || layer >= 5)
		return;

	int mousePosInTile_X = static_cast<int>(std::floor(mousePos.x / TILE_SIZE));
	int mousePosInTile_Y = static_cast<int>(std::floor(mousePos.y / TILE_SIZE));


	if (mousePosInTile_X >= 0 && mousePosInTile_X < MAP_WIDTH && mousePosInTile_Y >= 0 && mousePosInTile_Y < MAP_HEIGHT)
	{
		Entity newEntity(uniqueEntity);
		newEntity.posX = mousePos.x;
		newEntity.posY = mousePos.y;

		entities.push_back(newEntity);
	}




	return;
}

void Map::resetTile(sf::Vector2f mousePos, int brushSize) {
	int mousePosInTile_X = static_cast<int>(std::floor(mousePos.x / TILE_SIZE));
	int mousePosInTile_Y = static_cast<int>(std::floor(mousePos.y / TILE_SIZE));

	if (mousePosInTile_X < 0 || mousePosInTile_Y < 0)
		return;
	if (mousePosInTile_X >= MAP_WIDTH || mousePosInTile_Y >= MAP_HEIGHT)
		return;


	const sf::Texture emptyTexture;
	int forStart = -std::floor((brushSize - 0.1) / 2);
	int forEnd = std::floor(brushSize / 2);
	for (int i = forStart; i <= forEnd; ++i) {
		for (int j = forStart; j <= forEnd; ++j) {
			if (mousePosInTile_X + i >= 0 && mousePosInTile_X + i < MAP_WIDTH && mousePosInTile_Y + j >= 0 && mousePosInTile_Y + j < MAP_HEIGHT)
			{
				for (int k = 0; k <= 4; ++k) {
					tiles[mousePosInTile_X + i][mousePosInTile_Y + j][k].sprite.setTexture(emptyTexture);
					tiles[mousePosInTile_X + i][mousePosInTile_Y + j][k].id = -1;
				}
			}
		}
	}

}




void Map::removeActor(sf::Vector2f mousePos, int brushSize) {
	int mousePosInTile_X = static_cast<int>(std::floor(mousePos.x / TILE_SIZE));
	int mousePosInTile_Y = static_cast<int>(std::floor(mousePos.y / TILE_SIZE));

	if (mousePosInTile_X < 0 || mousePosInTile_Y < 0)
		return;
	if (mousePosInTile_X >= MAP_WIDTH || mousePosInTile_Y >= MAP_HEIGHT)
		return;


	int forStart = -std::floor((brushSize - 0.1) / 2);
	int forEnd = std::floor(brushSize / 2);
	for (int i = forStart; i <= forEnd; ++i) {
		for (int j = forStart; j <= forEnd; ++j) {
			if (mousePosInTile_X + i >= 0 && mousePosInTile_X + i < MAP_WIDTH && mousePosInTile_Y + j >= 0 && mousePosInTile_Y + j < MAP_HEIGHT)
			{
				entities.erase(
					std::remove_if(
						entities.begin(),
						entities.end(),
						[mousePosInTile_X, mousePosInTile_Y, i, j](const Entity& entity) {
							int entityTileX = static_cast<int>(std::floor(entity.posX / TILE_SIZE));
							int entityTileY = static_cast<int>(std::floor(entity.posY / TILE_SIZE));
							return entityTileX == mousePosInTile_X + i && entityTileY == mousePosInTile_Y + j;
						}
					),
					entities.end()
				); //JE PEUX SIMPLIFIER
			}
		}
	}

}



void Map::updateEntities(float dt)
{
	for (auto& entity : entities) {
		entity.update(dt);
	}
}

bool Map::checkCollision(const sf::FloatRect& bounds) const
{
	for (size_t layer = 0; layer < tiles.size(); ++layer) {
		for (size_t y = 0; y < tiles[layer].size(); ++y) {
			for (size_t x = 0; x < tiles[layer][y].size(); ++x) {
				const auto& tile = tiles[layer][y][x];
				if (tile.blockEntity) {
					if (bounds.intersects(tile.sprite.getGlobalBounds())) {
						return true; // Collision detected
					}
				}
			}
		}
	}
	return false; // No collision
}


void Map::handlePlayerAttack(const sf::FloatRect& attackBounds, int damage) {
	for (auto it = entities.begin(); it != entities.end(); ) {
		// Récupère la bounding box de l'entité
		sf::FloatRect entityBounds = it->uniqueEntity.sprite.getGlobalBounds();

		// Si l'attaque touche l'entité
		if (attackBounds.intersects(entityBounds)) {
			// Supposons que Unique_entity ou Entity possède un champ HP
			// (ou directement dans uniqueEntity.HP)
			it->HP -= damage;
			std::cerr << "Entity hit! Remaining HP: " << it->HP << std::endl;

			// Si les PV tombent à 0 ou moins, on supprime l'entité de la map
			if (it->HP <= 0) {
				std::cerr << "Entity killed and removed.\n";
				it = entities.erase(it); // Supprime proprement de la liste
				continue;
			}
		}
		++it;
	}
}



void Map::setEffect(int effectId, int posX, int posY, float scaleX, int layer)
{


	if (layer < 0 || layer >= 5)
		return;

	int mousePosInTile_X = static_cast<int>(std::floor(posX / TILE_SIZE));
	int mousePosInTile_Y = static_cast<int>(std::floor(posY / TILE_SIZE));

	if (mousePosInTile_X >= 0 && mousePosInTile_X < MAP_WIDTH && mousePosInTile_Y >= 0 && mousePosInTile_Y < MAP_HEIGHT)
	{

		Unique_effect uniqueEffect = spritesManager.getUniqueEffect(effectId);

		Effect newEffect(uniqueEffect);
		newEffect.posX = posX;
		newEffect.posY = posY;
		newEffect.uniqueEffect.sprite.setScale(scaleX, 1.f);

		effects.push_back(newEffect);
	}

	return;
}

void Map::updateEffects(float deltaTime) {

	for (auto& effect : effects)
	{
		effect.update(deltaTime);
	}

	effects.erase(
		std::remove_if(
			effects.begin(),
			effects.end(),
			[](const Effect& effect)
			{
				return effect.elapsedTimer >= effect.duration;
			}
		),
		effects.end()
	);
}