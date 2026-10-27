#include "sprites_manager.hpp"


Sprites_manager::Sprites_manager()
{
}

const Unique_tile& Sprites_manager::getUniqueTile(int id) const
{
	for (const Unique_tile& uniqueTile : allUniqueTiles) {
		if (uniqueTile.id == id) {
			return uniqueTile;
		}
	}
	// In case nothing is found, returns first sprite
	
	return allUniqueTiles.at(0);
}


const Unique_entity& Sprites_manager::getUniqueEntity(int id) const
{
    for (const Unique_entity& uniqueEntity : allUniqueEntities) {
        if (uniqueEntity.id == id) {
            return uniqueEntity;
        }
    }
    // In case nothing is found, returns first sprite

    return allUniqueEntities.at(0);
}


const Unique_effect& Sprites_manager::getUniqueEffect(int id) const
{
    std::cout << "getUniqueEffect avant for\n";

    for (const Unique_effect& uniqueEffect: allUniqueEffects) {
        std::cout << "getUniqueEffect debut for\n";

        if (uniqueEffect.id == id) {
            return uniqueEffect;
        }
    }
    // In case nothing is found, returns first sprite
    std::cout << "getUniqueEffect après for\n";

    return allUniqueEffects.at(0);
}








int Sprites_manager::askDefaultLayer(sf::RenderWindow& window)
{
    sf::Font font;
    if (!font.loadFromFile("ARIAL.TTF"))
    {
        std::cerr << "Impossible de charger la police.\n";
        return -1;
    }

    sf::RectangleShape background(sf::Vector2f(400.f, 180.f));
    background.setFillColor(sf::Color(40, 40, 40));
    background.setOutlineThickness(3.f);
    background.setOutlineColor(sf::Color::White);
	background.setPosition(SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2 - 90);

    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(22);
    title.setFillColor(sf::Color::White);
    title.setString("Default layer (0 - 4) :");
    title.setPosition(SCREEN_WIDTH / 2 - 200 + 30.f, SCREEN_HEIGHT / 2 - 90 + 25.f);

    sf::Text input;
    input.setFont(font);
    input.setCharacterSize(28);
    input.setFillColor(sf::Color::White);
    input.setPosition(SCREEN_WIDTH / 2 - 200 + 30.f, SCREEN_HEIGHT / 2 - 90 + 80.f);

    std::string text;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return -1;
            }

            if (event.type == sf::Event::TextEntered)
            {
                // Backspace
                if (event.text.unicode == 8)
                {
                    if (!text.empty())
                        text.pop_back();
                }

                // Figures only
                else if (event.text.unicode >= '0' &&
                    event.text.unicode <= '9')
                {
                    if (text.size() < 1)
                        text += static_cast<char>(event.text.unicode);
                }

                // Enter
                else if (event.text.unicode == 13)
                {
                    if (!text.empty())
                    {
                        int layer = std::stoi(text);

                        if (layer >= 0 && layer <= 4)
                            return layer;
                    }
                }
            }

            // Escape = cancel
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
            {
                return -1;
            }
        }

        input.setString(text);

        window.clear(sf::Color::Black);

        window.draw(background);
        window.draw(title);
        window.draw(input);

        window.display();
    }

    return -1;
}










void Sprites_manager::newTile(sf::RenderWindow& window, std::string selectedFile, int defaultLayer, bool blockEntity, bool blockLight) {


	if (selectedFile.empty()) {
		return; // User cancelled
	}

	auto texture = std::make_unique<sf::Texture>();

	std::cerr << "Chargement de la texture d'origine : " << selectedFile << std::endl;


	if (!texture->loadFromFile(selectedFile)) {
		std::cerr << "Erreur : impossible de charger la texture " << selectedFile << "\n";
		return;
	}

    if (defaultLayer == -1)
    {
        std::cerr << "newSprite() canceled.\n";
        return;
    }


	Unique_tile uniqueTile;
	uniqueTile.sprite.setTexture(*texture);
	std::cerr << allUniqueTiles.size() << std::endl;
	uniqueTile.id = static_cast<int>(allUniqueTiles.size());
	uniqueTile.filePath = selectedFile;
	uniqueTile.defaultLayer = defaultLayer;
	uniqueTile.blockEntity = blockEntity;
    uniqueTile.blockLight = blockLight;


	m_textures.push_back(std::move(texture));
	std::cerr << "newSprite done" << std::endl;

	allUniqueTiles.push_back(uniqueTile);
}



void Sprites_manager::newEntity(sf::RenderWindow& window, std::string selectedFile, int defaultLayer) {
    
    
    if (selectedFile.empty()) {
        return; // User cancelled
    }

    auto texture = std::make_unique<sf::Texture>();

    std::cerr << "Chargement de la texture d'origine de l'entity : " << selectedFile << std::endl;


    if (!texture->loadFromFile(selectedFile)) {
        std::cerr << "Erreur : impossible de charger la texture " << selectedFile << "\n";
        return;
    }

    if (defaultLayer == -1)
    {
        std::cerr << "newEntity() canceled.\n";
        return;
    }
    
    Unique_entity uniqueEntity;
    uniqueEntity.sprite.setTexture(*texture);
    std::cerr << allUniqueEntities.size() << std::endl;
    uniqueEntity.id = static_cast<int>(allUniqueEntities.size());
    uniqueEntity.filePath = selectedFile;
    uniqueEntity.defaultLayer = defaultLayer;

    m_textures.push_back(std::move(texture));
    std::cerr << "newEntity done" << std::endl;

    allUniqueEntities.push_back(uniqueEntity);
}



void Sprites_manager::saveSpriteSet(const std::string& filePath)
{
	std::cout << "saving the spriteSet" << std::endl;


	std::ofstream file(filePath);

	if (!file) {
		std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
		return;
	}

	file << "## SPRITE_SET SIZE ##\n" << allUniqueTiles.size() << "\n";
	file << "## SPRITE_SET DATA ##\n";

	for (Unique_tile uniqueTile : allUniqueTiles) {
        file << uniqueTile.defaultLayer << " " << uniqueTile.blockEntity << " " << uniqueTile.blockLight << " " << uniqueTile.filePath << "\n";

	}


	std::cout << "spriteSet sauvegardé\n";


}

void Sprites_manager::loadSpriteSet(sf::RenderWindow& window, const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file) {
		std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
		return;
	}

	std::cerr << "loadSpriteSet: " << filePath << " en lecture.\n";


	allUniqueTiles.clear();

	std::string line;
	int spriteSetSize = 0;

	while (std::getline(file, line))
	{
		if (line == "## SPRITE_SET SIZE ##") {
			std::getline(file, line);
			spriteSetSize = std::stoi(line);
			std::cout << "SIZE : " << spriteSetSize << std::endl;
		}
		if (line == "## SPRITE_SET DATA ##") {
            int x = 0;
			//tiles.assign(MAP_WIDTH, std::vector<std::vector<Unique_sprite>>(MAP_HEIGHT, std::vector<Unique_sprite>(5, Unique_sprite())));
			//tiles.resize(MAP_HEIGHT, std::vector<std::vector<int>>(MAP_WIDTH, std::vector<int>(5, 0))); //Use it to resize lol
			while (std::getline(file, line) && x < spriteSetSize) {
                std::istringstream rowIss(line);
                std::string block;
                std::getline(rowIss, block, ' ');
				int layer = std::stoi(block);
				std::cout << "block : " << block << std::endl;
                std::getline(rowIss, block, ' ');
                bool blockEntity;
                if (block == "1") blockEntity = true;
                else blockEntity = false;
                std::getline(rowIss, block, ' ');
                bool blockLight;
                if (block == "1") blockLight = true;
                else blockLight = false;
				std::getline(rowIss, block);
				std::string filePath = block;
				newTile(window, filePath, layer, blockEntity, blockLight);

				++x;
			}
		}
	}
}










void Sprites_manager::saveEntitySet(const std::string& filePath)
{
    std::cout << "saving the entitySet" << std::endl;


    std::ofstream file(filePath);

    if (!file) {
        std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
        return;
    }

    file << "## ENTITY_SET SIZE ##\n" << allUniqueEntities.size() << "\n";
    file << "## ENTITY_SET DATA ##\n";

    for (Unique_entity uniqueEntity : allUniqueEntities) {
        file << uniqueEntity.defaultLayer << " " << uniqueEntity.blockEntity << " " << uniqueEntity.blockLight << " " << uniqueEntity.filePath << "\n";

    }


    std::cout << "entitySet sauvegardé\n";


}

void Sprites_manager::loadEntitySet(sf::RenderWindow& window, const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
        return;
    }

    std::cerr << "loadEntitySet: " << filePath << " en lecture.\n";


    allUniqueEntities.clear();

    std::string line;
    int entitySetSize = 0;

    while (std::getline(file, line))
    {
        if (line == "## ENTITY_SET SIZE ##") {
            std::getline(file, line);
            entitySetSize = std::stoi(line);
            std::cout << "SIZE : " << entitySetSize << std::endl;
        }
        if (line == "## ENTITY_SET DATA ##") {
            int x = 0;
            while (std::getline(file, line) && x < entitySetSize) {
                std::istringstream rowIss(line);
                std::string block;
                std::getline(rowIss, block, ' ');
                int layer = std::stoi(block);
                std::cout << "block : " << block << std::endl;
                std::getline(rowIss, block, ' ');
                bool blockEntity;
                if (block == "1") blockEntity = true;
                else blockEntity = false;
                std::getline(rowIss, block, ' ');
                bool blockLight;
                if (block == "1") blockLight = true;
                else blockLight = false;
                std::getline(rowIss, block);
                std::string filePath = block;
                newEntity(window, filePath, layer);

                ++x;
            }
        }
    }
}













void Sprites_manager::newEffect(sf::RenderWindow& window, std::string selectedFile, int defaultLayer, int frameWidth, int frameHeight, int frameCount, float frameDuration, bool looping)
{
    if (selectedFile.empty())
        return;

    if (defaultLayer == -1)
    {
        std::cerr << "newEffect() canceled.\n";
        return;
    }

    // Vérification de la configuration.
    if (frameWidth < 0 || frameHeight < 0 || frameCount < 1 || frameDuration <= 0.f)
    {
        std::cerr << "Parametres d'animation invalides.\n";
        return;
    }

    auto texture = std::make_unique<sf::Texture>();

    if (!texture->loadFromFile(selectedFile))
    {
        std::cerr << "Erreur : impossible de charger la texture "
            << selectedFile << '\n';
        return;
    }

    const sf::Vector2u textureSize = texture->getSize();

    // Si aucune dimension n'est fournie, l'image est statique.
    if (frameWidth == 0 && frameHeight == 0)
    {
        frameWidth = static_cast<int>(textureSize.x);
        frameHeight = static_cast<int>(textureSize.y);
        frameCount = 1;
    }
    else if (frameWidth <= 0 || frameHeight <= 0 || frameWidth > static_cast<int>(textureSize.x) || frameHeight > static_cast<int>(textureSize.y))
    {
        std::cerr << "Dimensions de frame invalides.\n";
        return;
    }

    // Vérifier que les frames horizontales tiennent dans l'image.
    if (frameCount > static_cast<int>(textureSize.x) / frameWidth ||
        frameHeight > static_cast<int>(textureSize.y))
    {
        std::cerr << "Le nombre de frames depasse la spritesheet.\n";
        return;
    }

    Unique_effect uniqueEffect;

    uniqueEffect.sprite.setTexture(*texture);

    uniqueEffect.id = static_cast<int>(allUniqueEffects.size());
    uniqueEffect.filePath = selectedFile;
    uniqueEffect.defaultLayer = defaultLayer;

    uniqueEffect.frameWidth = frameWidth;
    uniqueEffect.frameHeight = frameHeight;
    uniqueEffect.frameCount = frameCount;
    uniqueEffect.frameDuration = frameDuration;
    uniqueEffect.looping = looping;

    uniqueEffect.sprite.setTextureRect(
        sf::IntRect(0, 0, frameWidth, frameHeight)
    );

    // La texture reste vivante tant que le gestionnaire la conserve.
    m_textures.push_back(std::move(texture));

    allUniqueEffects.push_back(uniqueEffect);

    std::cout << "Effet ajoute : "
        << uniqueEffect.name
        << " (" << frameCount << " frames)\n";
}



void Sprites_manager::loadEffectSet(sf::RenderWindow& window, const std::string& filePath)
{
    std::ifstream file(filePath);

    if (!file) {
        std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
        return;
    }

    std::cerr << "loadEffectSet: " << filePath << " en lecture.\n";


    allUniqueEffects.clear();

    std::string line;
    int effectSetSize = 0;

    while (std::getline(file, line))
    {
        if (line == "## EFFECT_SET SIZE ##") {
            std::getline(file, line);
            effectSetSize = std::stoi(line);
            std::cout << "SIZE : " << effectSetSize << std::endl;
        }
        if (line == "## EFFECT_SET DATA ##") {
            int x = 0;
            while (std::getline(file, line) && x < effectSetSize) {
                std::istringstream rowIss(line);
                std::string block;
                std::getline(rowIss, block, ' ');
                int layer = std::stoi(block);
                std::cout << "block : " << block << std::endl;
                std::getline(rowIss, block, ' ');
                bool blockEntity;
                if (block == "1") blockEntity = true;
                else blockEntity = false;
                std::getline(rowIss, block, ' ');
                bool blockLight;
                if (block == "1") blockLight = true;
                else blockLight = false;

                std::getline(rowIss, block, ' ');
                int effectWidth = std::stoi(block);

                std::getline(rowIss, block, ' ');
                int effectHeight = std::stoi(block);
                std::cout << "effectHeight : " << block << std::endl;

                std::getline(rowIss, block, ' ');
                int frameCount = std::stoi(block);
                std::cout << "effectHeight : " << block << std::endl;

                std::getline(rowIss, block, ' ');
                float frameDuration = std::stof(block);
                std::cout << "frameDuration : " << block << std::endl;

                std::getline(rowIss, block);
                std::string filePath = block;
                newEffect(window, filePath, layer, effectWidth, effectHeight, frameCount, frameDuration);
                std::cout << "Apres newEffect" << std::endl;


                ++x;
            }
        }
    }
}
