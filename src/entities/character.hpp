#pragma once

#include <SFML/graphics.hpp>
#include "randomNumber.hpp"
#include "hitbox.hpp"
#include "pausableClock.hpp"

class Character
{
public:
    Character();
    Character(sf::Vector2f position, sf::Vector2f size, int maxHealth, int minDamage, int maxDamage, int speed);

    // is called every tick
    virtual void tick() = 0;
    // movement method
    virtual void move() = 0;
    // drawing methode
    virtual void draw(sf::RenderWindow &window) = 0;
    // deltaTime value
    float deltaTime();

    // fighting stuff
    bool canAttack();
    void resetAttackCooldown();
    int attack();
    int doDamage();
    void takeDamage(int damage);
    void heal(int amount);
    void pause();
    void resume();

    // getter
    sf::Vector2f getPosition();
    sf::Vector2f getSize();
    Hitbox getHitbox();

    int getHealth();
    int getMaxHealth();
    int getSpeed();
    int getMinDamage();
    int getMaxDamage();
    sf::Time getAttackCooldownTime();

    // setter
    void setPosition(sf::Vector2f position);

protected:
    sf::Vector2f position;
    sf::Vector2f size;
    Hitbox hitbox;

    int maxHealth;
    int health;
    int minDamage;
    int maxDamage;
    int speed;

    PausableClock clock;

    sf::Clock attackCooldown;
    sf::Time attackSpeed;

    sf::Time pauseTime;
    sf::Time startTime;
};