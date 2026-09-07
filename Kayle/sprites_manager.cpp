#include "sprites_manager.hpp"


Sprites_manager::Sprites_manager()
{
}

const Unique_sprite& Sprites_manager::getSprite(int id) const
{
	Unique_sprite theSpriteWithThisId;
	for (const Unique_sprite& uniqueSprite : allUniqueSprites) {
		if (uniqueSprite.id == id) {
			return uniqueSprite;
		}
	}
	// In case nothing is found, returns first sprite
	
	return allUniqueSprites.at(0);
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










void Sprites_manager::newSprite(sf::RenderWindow& window, std::string selectedFile, int defaultLayer) {


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


	Unique_sprite uniqueSprite;
	uniqueSprite.sprite.setTexture(*texture);
	std::cerr << allUniqueSprites.size() << std::endl;
	uniqueSprite.id = static_cast<int>(allUniqueSprites.size());
	uniqueSprite.filePath = selectedFile;
	uniqueSprite.defaultLayer = defaultLayer;

	m_textures.push_back(std::move(texture));
	std::cerr << "newSprite done" << std::endl;

	allUniqueSprites.push_back(uniqueSprite);
}



void Sprites_manager::saveSpriteSet(const std::string& filePath)
{
	std::cout << "saving the spriteSet" << std::endl;


	std::ofstream file(filePath);

	if (!file) {
		std::cerr << "Erreur : impossible d'ouvrir " << filePath << " en lecture.\n";
		return;
	}

	file << "## SPRITE_SET SIZE ##\n" << allUniqueSprites.size() << "\n";
	file << "## SPRITE_SET DATA ##\n";

	for (Unique_sprite uniqueSprite : allUniqueSprites) {
		file << uniqueSprite.defaultLayer << ' ' << uniqueSprite.filePath << "\n";
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


	allUniqueSprites.clear();

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
				std::getline(rowIss, block);
				std::string filePath = block;
				newSprite(window, filePath, layer);

				++x;
			}
		}
	}
}
