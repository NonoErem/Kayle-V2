#include "editor_interface.hpp"
#include <SFML/Graphics.hpp>
#include "windows_things.hpp"


/*
Editor_interface::Editor_interface(sf::RenderWindow& window, Map& map, Sprites_manager& spritesManager) : theMap(map){

	
	if (!font.loadFromFile("ARIAL.TTF")) {
		std::cerr << "Erreur lors du chargement de la police ARIAL.TTF" << std::endl;
	}
	

	int posX = SCREEN_WIDTH * (1 - 0.2);
	int posY = SCREEN_HEIGHT * (1 - 0.9);
	int sizeX = SCREEN_WIDTH * 0.2;
	int sizeY = SCREEN_HEIGHT * 0.8;
	auto panel = std::make_unique<Interface_panel>(window, map, spritesManager, font, posX, posY, sizeX, sizeY);
	interfacePanels.push_back(std::move(panel));

}

void Editor_interface::displayInterface(sf::RenderWindow& window) {
	for (const auto& panelPtr : interfacePanels) {
		window.draw(panelPtr->shape);
		for (const auto& objectPtr : panelPtr->panelItems) {
			objectPtr->display(window);
		}
	}
}



Interface_panel::Interface_panel(sf::RenderWindow& window, Map& map, Sprites_manager& spritesManager, sf::Font& font, int posX, int posY, int sizeX, int sizeY) {
	shape.setSize(sf::Vector2f(sizeX, sizeY));
	//shape.setOutlineColor(sf::Color::Red);
	//shape.setOutlineThickness(5);
	shape.setPosition(posX, posY);

	//Line 1
	int posY_line1 = posY + sizeY * 0.01;
	int buttonSizeX = 110;
	int buttonSizeY = 40;
	float characterSize = 0.8;
	int posY_spriteSelector = posY + sizeY * 0.21;

	auto selector = std::make_unique<Sprite_selector>(window, spritesManager, font, posX + sizeX * 0.05, posY_spriteSelector, sizeX, sizeY * 0.7);
	Sprite_selector* selectorPtr = selector.get();
	auto addSpriteButton = std::make_unique<Interface_button>(window, map, "Add tile", font, [&, selectorPtr]() { spritesManager.newSprite(window); selectorPtr->update(); }, posX + sizeX * 0.05, posY_line1, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(std::move(addSpriteButton));
	panelItems.push_back(std::move(selector));
	auto saveButton = std::make_unique<Interface_button>(window, map, "Save", font, [&]() { BaseGameState = MAINMENU; }, posX + sizeX * 0.35, posY_line1, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(std::move(saveButton));
	auto loadButton = std::make_unique<Interface_button>(window, map, "Load", font, [&]() { map.loadFromFile("Map.txt"); }, posX + sizeX * 0.65, posY_line1, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(std::move(loadButton));

	//Line 2
	int posY_line2 = posY + sizeY * 0.11;
	auto paintButton = std::make_unique<Interface_button>(window, map, "Paint", font, [&]() { BaseGameState = MAINMENU; }, posX + sizeX * 0.05, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(std::move(paintButton));
	buttonSizeX = 70;
	characterSize = 0.4f;
	auto eraserAllButton = std::make_unique<Interface_button>(window, map, "Erase\nall", font, [&]() { BaseGameState = MAINMENU; }, posX + sizeX * 0.35, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(std::move(eraserAllButton));
	auto tileEraserButton = std::make_unique<Interface_button>(window, map, "Remove\ntiles", font, [&]() { map.loadFromFile("Map.txt"); }, posX + sizeX * 0.55, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(std::move(tileEraserButton));
	auto actorEraserButton = std::make_unique<Interface_button>(window, map, "Remove\nactors", font, [&]() { map.loadFromFile("Map.txt"); }, posX + sizeX * 0.75, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(std::move(actorEraserButton));

	//Sprites Selector
	//spriteSelector declared earlier for dependance issues
	std::cerr << "spriteSelector initialisé\n";


}

Interface_button::Interface_button(sf::RenderWindow& window, Map& map, std::string stringLabel, sf::Font& font, std::function<void()> action, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize) {
	
	shape.setSize(sf::Vector2f(buttonSizeX, buttonSizeY));
	shape.setPosition(posX, posY);
	shape.setFillColor(sf::Color::Blue);
	objectAction = action;

	label.setFont(font);
	label.setCharacterSize(buttonSizeY*characterSize);
	label.setFillColor(sf::Color::Red);

	label.setString(stringLabel);
	label.setStyle(sf::Text::Regular);
	//float textWidth = label.getLocalBounds().width;
	label.setPosition(posX,posY);

}



Sprite_selector::Sprite_selector(sf::RenderWindow& window, Sprites_manager& spritesManager, sf::Font& font, int posX, int posY, int sizeX, int sizeY): spritesManagerHere(spritesManager)
{
	pos_X = posX;
	pos_Y = posY;
	size_X = sizeX;
	size_Y = sizeY;
	spriteButtons.clear();
	sf::Sprite spriteButton;
	int endForLoop = std::min(4, static_cast<int>(spritesManagerHere.allSprites.size()));
	for (int i = 0 + cursorOffset; i <= endForLoop - 1 + cursorOffset; i++) {

		const sf::Texture* texturePtr = spritesManagerHere.allSprites[i].sprite.getTexture();

		if (texturePtr != nullptr) {
			sf::Sprite spriteButton;
			spriteButton.setTexture(*texturePtr);
			spriteButton.setPosition(pos_X, pos_Y + size_Y * 0.2f * i);
			spriteButtons.push_back(spriteButton);
		}
	}
	std::cerr << "Initialisation du sprite selector\n";

}

void Sprite_selector::update() {
	std::cerr << "Maj du sprite selector commencée\n";
	spriteButtons.clear();
	sf::Sprite spriteButton;
	int endForLoop = std::min(4, static_cast<int>(spritesManagerHere.allSprites.size()));
	for (int i = 0 + cursorOffset; i <= endForLoop - 1 + cursorOffset; i++) {
		
		const sf::Texture* texturePtr = spritesManagerHere.allSprites[i].sprite.getTexture();
		
		if (texturePtr != nullptr) {
			sf::Sprite spriteButton;
			spriteButton.setTexture(*texturePtr);
			spriteButton.setPosition(pos_X, pos_Y + size_Y * 0.2f * i);
			spriteButtons.push_back(spriteButton);
		}
	}
	std::cerr << "Maj du sprite selector terminée\n";

}


void Sprite_selector::display(sf::RenderWindow& window) const {
	for (const sf::Sprite sprite : spriteButtons) {
		window.draw(sprite);

		sf::RectangleShape debugBox;
		debugBox.setPosition(sprite.getPosition());
		debugBox.setSize(sf::Vector2f(32.f, 32.f)); // Taille arbitraire
		debugBox.setFillColor(sf::Color::Transparent);
		debugBox.setOutlineColor(sf::Color::Red);
		debugBox.setOutlineThickness(2.f);
		window.draw(debugBox);
	
	}
}

*/




Editor_interface::Editor_interface(Sprites_manager& spritesManager) : outlineBox(0, 0, TILE_SIZE, TILE_SIZE) {
	
	panel(spritesManager);
}


void Editor_interface::panel(Sprites_manager& spritesManager) {
	if (!font.loadFromFile("ARIAL.TTF")) {
		std::cerr << "Erreur lors du chargement de la police ARIAL.TTF" << std::endl;
	}



	int posX = SCREEN_WIDTH * (1 - 0.2);
	int posY = SCREEN_HEIGHT * (1 - 0.9);
	int sizeX = SCREEN_WIDTH * 0.2;
	int sizeY = SCREEN_HEIGHT * 0.8;

	shape.setSize(sf::Vector2f(sizeX, sizeY));
	shape.setPosition(posX, posY);



	//Line 1
	int posY_line1 = posY + sizeY * 0.01;
	int buttonSizeX = 110;
	int buttonSizeY = 40;
	float characterSize = 0.8;
	int posY_spriteSelector = posY + sizeY * 0.21;

	Button addSpriteButton("Add tile", font, posX + sizeX * 0.05, posY_line1, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(addSpriteButton);
	Button saveButton("Save", font, posX + sizeX * 0.35, posY_line1, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(saveButton);
	Button loadButton("Load", font, posX + sizeX * 0.65, posY_line1, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(loadButton);

	//Line 2
	int posY_line2 = posY + sizeY * 0.11;
	Button paintButton("Paint", font, posX + sizeX * 0.05, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(paintButton);
	buttonSizeX = 70;
	characterSize = 0.4f;
	Button eraserAllButton("Erase\nall", font, posX + sizeX * 0.35, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(eraserAllButton);
	Button tileEraserButton("Remove\ntiles", font, posX + sizeX * 0.55, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(tileEraserButton);
	Button actorEraserButton("Remove\nactors", font, posX + sizeX * 0.75, posY_line2, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(actorEraserButton);

	//Sprite Selector
	int spritesByPage = 5;
	int endForLoop = std::min(spritesByPage + cursorOffset, static_cast<int>(spritesManager.allUniqueSprites.size()));
	for (int i = cursorOffset; i < endForLoop; i++) {
		std::string label = std::to_string(spritesManager.allUniqueSprites[i].defaultLayer);
		Sprite_button spriteButton(label, font, posX + sizeX * 0.05, posY_spriteSelector + sizeY * 0.7 * 0.2f * (i - cursorOffset), TILE_SIZE, TILE_SIZE, characterSize);

		//const sf::Texture* texturePtr = spritesManager.allUniqueSprites[i].sprite.getTexture();

		//if (texturePtr != nullptr) {
		spriteButton.uniqueSprite = spritesManager.allUniqueSprites[i];
		spriteButton.uniqueSprite.sprite.setPosition(std::floor(posX + sizeX * 0.05), std::floor(posY_spriteSelector + sizeY * 0.7 * 0.2f * (i - cursorOffset)));
		//}
		panelItems.push_back(spriteButton);
	}

	//Layer line
	int posY_layerLine = posY + sizeY * 0.81;
	buttonSizeX = 40;
	Button defaultLayerButton("Deflt\nLayer", font, posX + sizeX * 0.05, posY_layerLine, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(defaultLayerButton);
	Button layer0Button(" Lay\n er 0", font, posX + sizeX * 0.20, posY_layerLine, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(layer0Button);
	Button layer1Button(" Lay\n er 1", font, posX + sizeX * 0.35, posY_layerLine, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(layer1Button);
	Button layer2Button(" Lay\n er 2", font, posX + sizeX * 0.50, posY_layerLine, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(layer2Button);
	Button layer3Button(" Lay\n er 3", font, posX + sizeX * 0.65, posY_layerLine, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(layer3Button);
	Button layer4Button(" Lay\n er 4", font, posX + sizeX * 0.80, posY_layerLine, buttonSizeX, buttonSizeY, characterSize);
	panelItems.push_back(layer4Button);


}

//Maybe I can do it another way in order to avoid update every frame
void Editor_interface::updatePanel(Sprites_manager& spritesManager) {
	panelItems.clear();

	panel(spritesManager);
}

void Editor_interface::displayPanel(sf::RenderWindow& window) const
{
	window.draw(shape);
	for (const auto& item : panelItems) {
		window.draw(item.shape);
		window.draw(item.uniqueSprite.sprite);
		window.draw(item.label);
	}
	window.draw(outlineBox.shape);
}

Items::Items() {
	shape.setFillColor(sf::Color::Blue);
}

Button::Button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize) {

	shape.setSize(sf::Vector2f(buttonSizeX, buttonSizeY));
	shape.setPosition(posX, posY);
	shape.setFillColor(sf::Color::Blue);

	label.setFont(font);
	label.setCharacterSize(buttonSizeY * characterSize);
	label.setFillColor(sf::Color::Red);

	label.setString(stringLabel);
	label.setStyle(sf::Text::Regular);
	label.setPosition(posX, posY);
}

Sprite_button::Sprite_button(std::string stringLabel, sf::Font& font, int posX, int posY, int buttonSizeX, int buttonSizeY, float characterSize)
{
	shape.setSize(sf::Vector2f(buttonSizeX, buttonSizeY));
	shape.setPosition(posX, posY);
	shape.setFillColor(sf::Color::Blue);

	label.setFont(font);
	label.setCharacterSize(buttonSizeY * characterSize);
	label.setFillColor(sf::Color::Red);

	label.setString(stringLabel);
	label.setStyle(sf::Text::Regular);
	label.setPosition(posX + TILE_SIZE, posY);


}


OutlineBox::OutlineBox(int posX, int posY, int sizeX, int sizeY) {
	shape.setPosition(posX - 2, posY - 2);
	shape.setSize(sf::Vector2f(sizeX, sizeY));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(2.f);

}
