#pragma once
#include <SFML/Graphics.hpp>
#include "hitbox.hpp"
#include "pausableClock.hpp"

class Projectile
{
public:
    Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition, int damage, int speed, int size, sf::Color color);
    Projectile();
    ~Projectile();

    void tick();
    void setAngle(sf::Vector2f targetPosition);

    Hitbox getHitbox();
    void setHitbox(Hitbox hitbox);

    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition();

    sf::CircleShape getBody();
    int getDamage();

private:
    sf::CircleShape body;
    PausableClock clock;
    Hitbox hitbox;

    int damage;
    int speed;
    sf::Vector2f angle;
};
