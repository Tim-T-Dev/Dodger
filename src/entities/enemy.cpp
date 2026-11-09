#include "enemy.hpp"
#include <iostream>

int Enemy::nextId = 1000;

Enemy::Enemy(std::vector<sf::Texture> &textures, sf::Vector2f position, sf::Vector2f size, int maxHealth, int minDamage, int maxDamage, int speed, int experience, Player *player)
{
    this->position = position;
    this->size = size;
    this->hitbox = Hitbox(position, size.x, size.y);

    this->maxHealth = maxHealth;
    this->health = maxHealth;

    this->minDamage = minDamage;
    this->maxDamage = maxDamage;
    this->speed = speed;

    this->player = player;
    loadTextures(textures);

    attackSpeed = sf::seconds(0.5f);
    clock.reset();
    this->experience = experience;

    id = getNextId();
    active = true;
}

Enemy::~Enemy() {}

bool Enemy::operator==(const Enemy &other) const
{
    return id == other.id;
}

void Enemy::tick()
{
    if (active)
    {
        enemyDeltaTime = deltaTime();
        move();
        animationHandler();
    }
}

void Enemy::move()
{
    float movement = speed * enemyDeltaTime;
    sf::Vector2f positionChange(0, 0);

    // right from player
    if (getPosition().x > player->getPosition().x + (player->getHitbox().getWidth() / 2))
    {
        positionChange.x = -movement;
    }

    // left from player
    else if (getPosition().x + getSize().x < player->getPosition().x + (player->getHitbox().getWidth() / 2))
    {
        positionChange.x = movement;
    }

    // under player
    if (getPosition().y > player->getPosition().y + (player->getHitbox().getHeight() / 2))
    {
        positionChange.y = -movement;
    }

    // above player
    else if (getPosition().y + getSize().y < player->getPosition().y + (player->getHitbox().getHeight() / 2))
    {
        positionChange.y = movement;
    }

    setPosition(sf::Vector2f(positionChange.x + getPosition().x,
                             positionChange.y + getPosition().y));

    enemySprite.setPosition(sf::Vector2f(positionChange.x + enemySprite.getPosition().x,
                                         positionChange.y + enemySprite.getPosition().y));
}

void Enemy::draw(sf::RenderWindow &window)
{
    if (active)
    {
        window.draw(enemySprite);
        drawHealthbar(window);
    }
    // DEBUG hitbox.draw(window);
}

void Enemy::drawHealthbar(sf::RenderWindow &window)
{
    float sizePercentage = static_cast<float>(health) / static_cast<float>(maxHealth);

    sf::Vector2f barSize = sf::Vector2f(hitbox.getWidth(), 3);
    sf::Vector2f barPosition = sf::Vector2f(hitbox.getPosition().x, hitbox.getPosition().y - 6);

    sf::RectangleShape healthBackground(barSize);
    sf::RectangleShape healthBar(sf::Vector2f(barSize.x * sizePercentage, barSize.y));

    healthBackground.setPosition(barPosition);
    healthBar.setPosition(barPosition);

    healthBackground.setFillColor(sf::Color(50, 50, 50, 160));
    healthBar.setFillColor(sf::Color(230, 60, 60, 160));

    window.draw(healthBackground);
    window.draw(healthBar);
}

sf::Sprite Enemy::getBody()
{
    return enemySprite;
}

int Enemy::getNextId()
{
    return nextId++;
}

int Enemy::getId()
{
    return id;
}

void Enemy::loadTextures(std::vector<sf::Texture> &textures)
{
    animator.setTextures(textures);
    animator.setSpeed(4.0f);
    enemySprite.setTexture(animator.getTexture(0));
    enemySprite.setScale(0.5, 0.5);
}

void Enemy::setTexturePosition(int tweakPosX, int tweakPosY, int width, int height)
{
    enemySprite.setPosition(hitbox.getPosition());
    enemySprite.setPosition(enemySprite.getPosition().x + tweakPosX,
                            enemySprite.getPosition().y + tweakPosY);

    hitbox.setWidth(hitbox.getWidth() + width);
    hitbox.setHeight(hitbox.getHeight() + height);
}

void Enemy::animationHandler()
{
    if (player->getPosition().x > getPosition().x && mirrored)
    {
        mirrorTexture();
    }
    else if (player->getPosition().x < getPosition().x && !mirrored)
    {
        mirrorTexture();
    }

    updateAnimation();
}

void Enemy::updateAnimation()
{
    animator.updateAnimation(enemyDeltaTime);
    enemySprite.setTexture(animator.getCurrentTexture());
}

void Enemy::mirrorTexture()
{
    if (!mirrored)
    {
        enemySprite.setScale(-0.5, 0.5);
        enemySprite.setPosition(
            sf::Vector2f(enemySprite.getPosition().x + enemySprite.getGlobalBounds().getSize().x,
                         enemySprite.getPosition().y));
        mirrored = true;
    }
    else if (mirrored)
    {
        enemySprite.setScale(0.5, 0.5);
        enemySprite.setPosition(
            sf::Vector2f(enemySprite.getPosition().x - enemySprite.getGlobalBounds().getSize().x,
                         enemySprite.getPosition().y));
        mirrored = false;
    }
}

void Enemy::setInactive()
{
    active = false;
    setPosition(sf::Vector2f(-10000, -10000));
}

bool Enemy::isActive()
{
    return active;
}

int Enemy::getExp()
{
    return experience;
}
