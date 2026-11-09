#include "player.hpp"
#include <iostream>

Player::Player() {}

Player::Player(sf::Vector2f position, int maxHealth, int minDamage, int maxDamage, int speed)
{
    this->position = position;
    this->maxHealth = maxHealth;
    this->health = maxHealth;

    this->minDamage = minDamage;
    this->maxDamage = maxDamage;
    this->speed = speed;

    loadPlayerTextures();
    this->hitbox = Hitbox(position, 40, 40);
    setTexturePosition();

    attackSpeed = sf::seconds(0.8f);
    up = false;
    down = false;
    left = false;
    right = false;

    experienceStart = 0;
    experience = 0;
    experienceNeeded = 200;
    level = 0;
}

Player::~Player() {}

void Player::tick()
{
    playerDeltaTime = deltaTime();
    move();
    animationHandler();
}

void Player::move()
{
    float movement = speed * playerDeltaTime;
    sf::Vector2f positionChange(0, 0);

    // MOVE UP
    if (up && getPosition().y > 0)
    {
        positionChange.y -= movement;
    }
    else if (up)
    {
        setPosition(sf::Vector2f(getPosition().x, 0));
    }

    // MOVE DOWN
    if (down && getPosition().y < (verticalBorder - size.y))
    {
        positionChange.y += movement;
    }
    else if (down)
    {
        setPosition(sf::Vector2f(getPosition().x, verticalBorder - size.y));
    }

    // MOVE LEFT
    if (left && getPosition().x > 0)
    {
        positionChange.x -= movement;
    }
    else if (left)
    {
        setPosition(sf::Vector2f(0, getPosition().y));
    }

    // MOVE RIGHT
    if (right && getPosition().x < (horizontalBorder - size.x))
    {
        positionChange.x += movement;
    }
    else if (right)
    {
        setPosition(sf::Vector2f(horizontalBorder - size.x, getPosition().y));
    }

    // new position
    setPosition(sf::Vector2f(positionChange.x + getPosition().x,
                             positionChange.y + getPosition().y));

    playerSprite.setPosition(sf::Vector2f(positionChange.x + playerSprite.getPosition().x,
                                          positionChange.y + playerSprite.getPosition().y));
}

void Player::draw(sf::RenderWindow &window)
{
    window.draw(playerSprite);
    // DEBUG hitbox.draw(window);
}

void Player::setMoveDirection(Direction direction)
{
    switch (direction)
    {
    case Direction::UP:
        up = true;
        break;
    case Direction::DOWN:
        down = true;
        break;
    case Direction::LEFT:
        left = true;
        break;
    case Direction::RIGHT:
        right = true;
        break;
    default:
        break;
    }
}

void Player::stopMoveDirection(Direction direction)
{
    switch (direction)
    {
    case Direction::UP:
        up = false;
        break;
    case Direction::DOWN:
        down = false;
        break;
    case Direction::LEFT:
        left = false;
        break;
    case Direction::RIGHT:
        right = false;
        break;
    default:
        break;
    }
}

void Player::setBounds(int vertical, int horizontal)
{
    verticalBorder = vertical;
    horizontalBorder = horizontal;
}

void Player::loadPlayerTextures()
{
    std::vector<std::string> filePaths;
    filePaths.push_back("resources/player/frame0.png");
    filePaths.push_back("resources/player/frame1.png");
    filePaths.push_back("resources/player/frame2.png");
    filePaths.push_back("resources/player/frame3.png");
    animator.loadTextures(filePaths);
    animator.setSpeed(5.0f);

    playerSprite.setTexture(animator.getTexture(0));
    playerSprite.setScale(0.5, 0.5);
}

void Player::setTexturePosition()
{
    playerSprite.setPosition(hitbox.getPosition());

    sf::Vector2f pos;
    pos.x = playerSprite.getPosition().x - 11;
    pos.y = playerSprite.getPosition().y - 4;
    playerSprite.setPosition(pos);

    hitbox.setWidth(hitbox.getWidth() - 6);
    hitbox.setHeight(hitbox.getHeight() + 9);
    size.x = hitbox.getWidth();
    size.y = hitbox.getHeight();

    setPosition(sf::Vector2f(getPosition().x - 17,
                             getPosition().y - 16));

    playerSprite.setPosition(sf::Vector2f(playerSprite.getPosition().x - 20,
                                          playerSprite.getPosition().y - 20));
}

void Player::animationHandler()
{
    checkCharacterState();
    handleCharacterState();

    if (characterState != 3)
    {
        updateAnimation();
    }
}

void Player::updateAnimation()
{
    animator.updateAnimation(playerDeltaTime);
    playerSprite.setTexture(animator.getCurrentTexture());
}

void Player::mirrorTexture()
{
    if (!mirrored)
    {
        playerSprite.setScale(-0.5, 0.5);
        playerSprite.setPosition(
            sf::Vector2f(playerSprite.getPosition().x + playerSprite.getGlobalBounds().getSize().x,
                         playerSprite.getPosition().y));
        mirrored = true;
    }
    else if (mirrored)
    {
        playerSprite.setScale(0.5, 0.5);
        playerSprite.setPosition(
            sf::Vector2f(playerSprite.getPosition().x - playerSprite.getGlobalBounds().getSize().x,
                         playerSprite.getPosition().y));
        mirrored = false;
    }
}

void Player::checkCharacterState()
{
    // Check current state
    if (right == true && left == false)
    {
        characterState = 0;
    }
    else if (right == false && left == true)
    {
        characterState = 1;
    }
    else if (up != down)
    {
        characterState = 2;
    }
    else if (right == false && left == false && up == false && down == false)
    {
        characterState = 3;
    }
}

void Player::handleCharacterState()
{
    // Handle state MOVING RIGHT
    if (characterState == 0 && mirrored)
    {
        mirrorTexture();
    }

    // Handle state MOVING LEFT
    if (characterState == 1 && !mirrored)
    {
        mirrorTexture();
    }

    // Handle state MOVING UP OR DOWN
    // Do nothing

    // Handle state IDLE
    if (characterState == 3)
    {
        playerSprite.setTexture(animator.getTexture(0));
    }
}

int Player::getExperienceStart()
{
    return experienceStart;
}

void Player::addExperience(int exp)
{
    experience += exp;
    if (experience >= experienceNeeded)
    {
        level++;
        experienceStart = experienceNeeded;
        experienceNeeded = (experienceNeeded * 1.2f) + 200;
    }
}

int Player::getExperience()
{
    return experience;
}

int Player::getExperienceNeeded()
{
    return experienceNeeded;
}

int Player::getLevel()
{
    return level;
}

void Player::setMinDamage(int damage)
{
    minDamage = damage;
}

void Player::setMaxDamage(int damage)
{
    maxDamage = damage;
}

void Player::setSpeed(int speed)
{
    this->speed = speed;
}

void Player::setMaxHealth(int health)
{
    maxHealth = health;
}

void Player::setAttackSpeed(sf::Time attackSpeed)
{
    this->attackSpeed = attackSpeed;
}
