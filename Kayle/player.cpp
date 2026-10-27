#include "player.hpp"

Player::Player(Map& mapHere) : map(mapHere) {

    textureIdle[0].loadFromFile("Textures/player/back_player_A.png");
    textureIdle[1].loadFromFile("Textures/player/back_player_B.png");

    textureDown[0].loadFromFile("Textures/player/front_player_A.png");
    textureDown[1].loadFromFile("Textures/player/front_player_B.png");

    textureUp[0].loadFromFile("Textures/player/back_player_A.png");
    textureUp[1].loadFromFile("Textures/player/back_player_B.png");

    textureLeft[0].loadFromFile("Textures/player/left_player_A.png");
    textureLeft[1].loadFromFile("Textures/player/left_player_B.png");

    textureRight[0].loadFromFile("Textures/player/right_player_A.png");
    textureRight[1].loadFromFile("Textures/player/right_player_B.png");

    textureMeleeDown[0].loadFromFile("Textures/player/front_player_A.png");
    textureMeleeDown[1].loadFromFile("Textures/player/front_player_A.png");

    textureMeleeUp[0].loadFromFile("Textures/player/back_player_A.png");
    textureMeleeUp[1].loadFromFile("Textures/player/back_player_A.png");

    textureMeleeLeft[0].loadFromFile("Textures/player/melee_left_player_A.png");
    textureMeleeLeft[1].loadFromFile("Textures/player/melee_left_player_B.png");

    textureMeleeRight[0].loadFromFile("Textures/player/melee_right_player_A.png");
    textureMeleeRight[1].loadFromFile("Textures/player/melee_right_player_B.png");



    // Taille et couleur héritées de Entity constructor
    sprite.setScale(1, 1);
    sprite.setTexture(textureIdle[0]);
    sprite.setPosition(posX, posY);
    hitbox.left = posX;
    hitbox.top = posY;

}

void Player::playerUpdate()
{
    changeTexture();
    floatingMovement();
	attackCooldownLeft -= deltaTime;
	actionDurationLeft -= deltaTime;
    switch (currentDirection) {
    case RIGHT:
        frameDuration = normalFrameDuration;
        break;
    case LEFT:
        frameDuration = normalFrameDuration;
        break;
    case DOWN:
        frameDuration = normalFrameDuration;
        break;
    case UP:
        frameDuration = normalFrameDuration;
        break;
    case RIGHT_MELEE:
        frameDuration = meleeFrameDuration;
        if (actionDurationLeft <= 0) {
            currentDirection = RIGHT;
        }
        break;
    case LEFT_MELEE:
        frameDuration = meleeFrameDuration;
        if (actionDurationLeft <= 0) {
            currentDirection = LEFT;
        }
        break;
    case DOWN_MELEE:
        frameDuration = meleeFrameDuration;
        if (actionDurationLeft <= 0) {
            currentDirection = DOWN;
        }
        break;
    case UP_MELEE:
        frameDuration = meleeFrameDuration;
        if (actionDurationLeft <= 0) {
            currentDirection = UP;
        }
        break;
	default:
		frameDuration = normalFrameDuration;
		break;
    }


}

sf::FloatRect Player::getAttackBounds() const {
    sf::Vector2f pos = hitbox.getPosition();
    float attackSize = TILE_SIZE/2.f;

    switch (currentDirection) {
    case RIGHT_MELEE:
        return sf::FloatRect(pos.x + TILE_SIZE, pos.y, attackSize, TILE_SIZE);
    case LEFT_MELEE:
        return sf::FloatRect(pos.x - attackSize, pos.y, attackSize, TILE_SIZE);
    case DOWN_MELEE:
        return sf::FloatRect(pos.x, pos.y + TILE_SIZE, TILE_SIZE, attackSize);
    case UP_MELEE:
        return sf::FloatRect(pos.x, pos.y - attackSize, TILE_SIZE, attackSize);
    default:
        return sf::FloatRect(0, 0, 0, 0); // Pas d'attaque en cours
    }
}


void Player::move(float moveX, float moveY) {
    float total = std::sqrt(moveX * moveX + moveY * moveY);
    if (total == 0) return;
    moveX /= total;
    moveY /= total;
    sf::Vector2f movement;
    movement = sf::Vector2f(moveX, moveY);
    movement *= effectiveSpeed * deltaTime;
    sprite.move(movement);
	posX = posX + movement.x;
	posY = posY + movement.y;
	hitbox.left = posX;
	hitbox.top = posY;
}

void Player::baseAttack(sf::Vector2f mouseWorld)
{
    if (attackCooldownLeft > 0.f) return;

    if (abs(mouseWorld.x - sprite.getPosition().x) >=
        abs(mouseWorld.y - sprite.getPosition().y))
    {
        if (mouseWorld.x >= sprite.getPosition().x)
            currentDirection = RIGHT_MELEE;
        else
            currentDirection = LEFT_MELEE;
    }
    else
    {
        if (mouseWorld.y >= sprite.getPosition().y)
            currentDirection = DOWN_MELEE;
        else
            currentDirection = UP_MELEE;
    }

    sf::FloatRect attackArea = getAttackBounds();
    map.handlePlayerAttack(attackArea, 50);
    switch (currentDirection) {
	case RIGHT_MELEE:
		//sprite.setTexture(textureMeleeRight[0]);
        map.setEffect(0, sprite.getPosition().x + TILE_SIZE*1.5, sprite.getPosition().y, -1, 2);
        break;
	case LEFT_MELEE:
		//sprite.setTexture(textureMeleeLeft[0]);
        map.setEffect(0, sprite.getPosition().x - TILE_SIZE*0.5, sprite.getPosition().y, 1, 2);

		break;
	case DOWN_MELEE:
		//sprite.setTexture(textureMeleeDown[0]);
        map.setEffect(0, sprite.getPosition().x - TILE_SIZE * 0.5, sprite.getPosition().y, 1, 2);

		break;
	case UP_MELEE:
		//sprite.setTexture(textureMeleeUp[0]);
        map.setEffect(0, sprite.getPosition().x - TILE_SIZE * 0.5, sprite.getPosition().y, 1, 2);

        break;
    default:
        //sprite.setTexture(textureMeleeRight[0]);
        map.setEffect(0, sprite.getPosition().x - TILE_SIZE * 0.5, sprite.getPosition().y, 1, 2);

        break;
    }

    attackCooldownLeft = attackCooldown;
    actionDurationLeft = meleeDuration;
    frameElapsed = 0.f;
    frameIndex = 0;
    frameDuration = 0.3f;
}

void Player::changeTexture() {


    frameElapsed += deltaTime;
    if (frameElapsed >= frameDuration)
    {
        frameElapsed = 0;
        frameIndex += 1;
        if (frameIndex > 1) {
            frameIndex = 0;
        }
        std::cerr << frameIndex << std::endl;
    }

    switch (currentDirection) {
    case DOWN:
        sprite.setTexture(textureDown[frameIndex]);
        break;
    case UP:
        sprite.setTexture(textureUp[frameIndex]);
        break;
    case LEFT:
        sprite.setTexture(textureLeft[frameIndex]);
        break;
    case RIGHT:
        sprite.setTexture(textureRight[frameIndex]);
        break;

    case DOWN_MELEE:
        sprite.setTexture(textureMeleeDown[frameIndex]);
        break;
    case UP_MELEE:
        sprite.setTexture(textureMeleeUp[frameIndex]);
        break;
    case LEFT_MELEE:
        sprite.setTexture(textureMeleeLeft[frameIndex]);
        break;
    case RIGHT_MELEE:
        sprite.setTexture(textureMeleeRight[frameIndex]);
        break;

    default:
        sprite.setTexture(textureIdle[frameIndex]);
        break;
    }
}

void Player::floatingMovement()
{
	if (currentDirection != IDLE && currentDirection != UP && currentDirection != DOWN && currentDirection != LEFT && currentDirection != RIGHT)
	{
		return;
	}
    float floatingAmplitude = 5.f;

    // Progression de l'animation entre 0 et 1
    float progress = frameElapsed / frameDuration;

    // Éviter les valeurs en dehors de l'intervalle attendu
    progress = std::clamp(progress, 0.f, 1.f);

    float offsetY;

    if (frameIndex == 0)
    {
        // Le sprite monte progressivement
        offsetY = floatingAmplitude * progress;
    }
    else
    {
        // Le sprite redescend progressivement
        offsetY = floatingAmplitude * (1.f - progress);
    }

    sprite.setPosition(posX, posY - offsetY);
}