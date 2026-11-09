#include "projectile.hpp"
#include <iostream>

Projectile::Projectile(sf::Vector2f startPosition, sf::Vector2f targetPosition, int damage, int speed, int size, sf::Color color)
{
    body = sf::CircleShape(size);
    body.setPosition(startPosition);
    body.setFillColor(color);

    hitbox = Hitbox(startPosition, size * 2, size * 2);

    setAngle(targetPosition);
    this->damage = damage;
    this->speed = speed;
}

Projectile::Projectile() {}

Projectile::~Projectile() {}

void Projectile::tick()
{
    sf::Time deltaTime = clock.getElapsedTime();
    float movement = speed * deltaTime.asSeconds();

    setPosition(sf::Vector2f(getPosition().x + (movement * angle.x), getPosition().y + (movement * angle.y)));
}

sf::CircleShape Projectile::getBody()
{
    return body;
}

int Projectile::getDamage()
{
    return damage;
}

void Projectile::setAngle(sf::Vector2f targetPosition)
{
    sf::Vector2f startingPosition = body.getPosition();

    float distanceX;
    float distanceY;
    float total;

    float partX;
    float partY;

    distanceX = std::max(targetPosition.x, startingPosition.x + body.getRadius()) - std::min(targetPosition.x, startingPosition.x + body.getRadius());
    distanceY = std::max(targetPosition.y, startingPosition.y + body.getRadius()) - std::min(targetPosition.y, startingPosition.y + body.getRadius());
    total = distanceX + distanceY;

    partX = distanceX / total;
    partY = distanceY / total;

    if (targetPosition.x < startingPosition.x)
    {
        partX *= -1;
    }

    if (targetPosition.y < startingPosition.y)
    {
        partY *= -1;
    }

    angle = sf::Vector2f(partX, partY);
}

Hitbox Projectile::getHitbox()
{
    return hitbox;
}

void Projectile::setHitbox(Hitbox hitbox)
{
    hitbox = hitbox;
}

void Projectile::setPosition(sf::Vector2f position)
{
    body.setPosition(position);
    hitbox.setPosition(position);
}

sf::Vector2f Projectile::getPosition()
{
    return body.getPosition();
}
