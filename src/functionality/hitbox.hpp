#pragma once
#include <SFML/Graphics.hpp>
#include "direction.hpp"

class Hitbox
{
public:
    Hitbox();
    Hitbox(sf::Vector2f position, int width, int height);
    ~Hitbox();

    // Check if this hitbox collides with an other hitbox
    bool collision(Hitbox other);
    Direction collisionDirection(Hitbox other);
    Direction fullCollisionCheck(Hitbox other);

    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f position);

    int getWidth();
    void setWidth(int width);

    int getHeight();
    void setHeight(int height);

    void draw(sf::RenderWindow &window);

private:
    sf::Vector2f position;
    int width;
    int height;
};