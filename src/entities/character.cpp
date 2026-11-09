#include "character.hpp"

Character::Character()
{
    position = sf::Vector2f(0, 0);
    size = sf::Vector2f(0, 0);
    hitbox = Hitbox(sf::Vector2f(0, 0), 0, 0);

    maxHealth = 0;
    health = 0;

    minDamage = 0;
    maxDamage = 0;
    speed = 0;

    attackSpeed = sf::seconds(1.0f);
}

Character::Character(sf::Vector2f position, sf::Vector2f size, int maxHealth, int minDamage, int maxDamage, int speed)
{
    this->position = position;
    this->size = size;
    this->hitbox = Hitbox(position, size.x, size.y);

    this->maxHealth = maxHealth;
    this->health = maxHealth;

    this->minDamage = minDamage;
    this->maxDamage = maxDamage;
    this->speed = speed;

    attackSpeed = sf::seconds(1.0f);
}

float Character::deltaTime()
{
    sf::Time deltaTime = clock.getElapsedTime();
    return deltaTime.asSeconds();
}

sf::Vector2f Character::getPosition()
{
    return position;
}

sf::Vector2f Character::getSize()
{
    return size;
}

Hitbox Character::getHitbox()
{
    return hitbox;
}

int Character::getHealth()
{
    return health;
}

int Character::getMaxHealth()
{
    return maxHealth;
}

int Character::getSpeed()
{
    return speed;
}

int Character::getMinDamage()
{
    return minDamage;
}

int Character::getMaxDamage()
{
    return maxDamage;
}

sf::Time Character::getAttackCooldownTime()
{
    return attackSpeed;
}

int Character::doDamage()
{
    return RandomNumber::getRandomInt(minDamage, maxDamage);
}

void Character::setPosition(sf::Vector2f position)
{
    this->position = position;
    hitbox.setPosition(position);
}

void Character::takeDamage(int damage)
{
    health -= damage;
    if (health < 0)
    {
        health = 0;
    }
}

void Character::heal(int amount)
{
    health += amount;
    if (health > maxHealth)
    {
        health = maxHealth;
    }
}

bool Character::canAttack()
{
    if ((pauseTime + attackCooldown.getElapsedTime() - startTime) >= attackSpeed)
    {
        pauseTime = sf::Time::Zero;
        startTime = sf::Time::Zero;
        return true;
    }
    return false;
}

void Character::resetAttackCooldown()
{
    attackCooldown.restart();
}

int Character::attack()
{
    if (canAttack())
    {
        resetAttackCooldown();
        return doDamage();
    }
    return 0;
}

void Character::pause()
{
    pauseTime = attackCooldown.getElapsedTime();
}

void Character::resume()
{
    startTime = attackCooldown.getElapsedTime();
}