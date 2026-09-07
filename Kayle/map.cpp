#include "map.hpp"



Map::Map(Sprites_manager& spritesManager) : spritesManager(spritesManager) {
	tiles.resize(1, std::vector<std::vector<Unique_sprite>>(1, std::vector<Unique_sprite>(5, Unique_sprite())));
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
			tiles.assign(MAP_WIDTH, std::vector<std::vector<Unique_sprite>>(MAP_HEIGHT, std::vector<Unique_sprite>(5, Unique_sprite())));
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
							if (v1 != -1) tiles[x][y][0] = spritesManager.getSprite(v1);
							if (v2 != -1) tiles[x][y][1] = spritesManager.getSprite(v2);
							if (v3 != -1) tiles[x][y][2] = spritesManager.getSprite(v3);
							if (v4 != -1) tiles[x][y][3] = spritesManager.getSprite(v4);
							if (v5 != -1) tiles[x][y][4] = spritesManager.getSprite(v5); //For whatever reason, on the older version I decided to put 5 layers instead of 3. I have to think about the saving format.
							++x;

						}
					}
					std::cout << block << std::endl;
				}
				++y;
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

	std::cout << "Carte sauvegardée\n";

}





void Map::drawLayer(sf::RenderWindow& window, int layer) {

	for (size_t x = 0; x < tiles.size(); ++x)
	{
		for (size_t y = 0; y < tiles[x].size(); ++y)
		{
			tiles[x][y][layer].sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);

			window.draw(tiles[x][y][layer].sprite);
		}
	}
}



void Map::displayMap(sf::RenderWindow& window) {

	for (size_t layer = 0; layer < tiles[0][0].size(); ++layer)
	{
		drawLayer(window, layer);
	}

}










void Map::setTile(Unique_sprite& uniqueSprite, sf::Vector2f mousePos, int layer)
{


	//ADD DIFFERENT BRUSH SIZES

	int mousePosInTile_X = static_cast<int>(std::floor(mousePos.x / TILE_SIZE));
	int mousePosInTile_Y = static_cast<int>(std::floor(mousePos.y / TILE_SIZE));


	size_t currentHeight = 0;

	if (!tiles.empty())
		currentHeight = tiles[0].size();


	if (mousePosInTile_Y < 0) {
		size_t rowsToAdd = static_cast<size_t>(-mousePosInTile_Y);

		for (auto& column : tiles)
		{
			column.insert(column.begin(), rowsToAdd, std::vector<Unique_sprite>(5));
		}

		MAP_HEIGHT += rowsToAdd;
		mousePosInTile_Y = 0;
	}
	
	if (mousePosInTile_X < 0)
	{
		size_t columnsToAdd = static_cast<size_t>(-mousePosInTile_X);
		tiles.insert(tiles.begin(), columnsToAdd, std::vector<std::vector<Unique_sprite>>(currentHeight, std::vector<Unique_sprite>(5)));

		mousePosInTile_X = 0;
		MAP_WIDTH += columnsToAdd;
	}

	if (mousePosInTile_X >= tiles.size()) {
		tiles.resize(mousePosInTile_X + 1, std::vector<std::vector<Unique_sprite>>(currentHeight, std::vector<Unique_sprite>(5)));
	}
	for (auto& column : tiles)
	{
		if (mousePosInTile_Y >= static_cast<int>(column.size()))
			column.resize(mousePosInTile_Y + 1, std::vector<Unique_sprite>(5));
	}


	tiles[mousePosInTile_X][mousePosInTile_Y][layer] = uniqueSprite;

	MAP_WIDTH = std::max(MAP_WIDTH, static_cast<int>(tiles.size()));
	MAP_HEIGHT = std::max(MAP_HEIGHT, static_cast<int>(tiles[0].size()));



	return;
}

void Map::resetTile(sf::Vector2f mousePos) {
	int mousePosInTile_X = static_cast<int>(std::floor(mousePos.x / TILE_SIZE));
	int mousePosInTile_Y = static_cast<int>(std::floor(mousePos.y / TILE_SIZE));

	if (mousePosInTile_X < 0 || mousePosInTile_Y < 0)
		return;
	if (mousePosInTile_X >= MAP_WIDTH || mousePosInTile_Y >= MAP_HEIGHT)
		return;


	const sf::Texture emptyTexture;
	for (int i = 0; i <= 4; ++i) {
		tiles[mousePosInTile_X][mousePosInTile_Y][i].sprite.setTexture(emptyTexture);
		tiles[mousePosInTile_X][mousePosInTile_Y][i].id = -1;
	}


}