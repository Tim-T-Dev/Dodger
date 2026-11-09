#include "hitbox.hpp"
#include <iostream>

Hitbox::Hitbox()
{
    position = sf::Vector2f(0, 0);
    width = 0;
    height = 0;
}

Hitbox::Hitbox(sf::Vector2f position, int width, int height)
{
    this->position = position;
    this->width = width;
    this->height = height;
}

Hitbox::~Hitbox() {}

sf::Vector2f Hitbox::getPosition()
{
    return position;
}

void Hitbox::setPosition(sf::Vector2f position)
{
    this->position = position;
}

int Hitbox::getWidth()
{
    return width;
}

void Hitbox::setWidth(int width)
{
    this->width = width;
}

int Hitbox::getHeight()
{
    return height;
}

void Hitbox::setHeight(int height)
{
    this->height = height;
}

void Hitbox::draw(sf::RenderWindow &window)
{
    sf::RectangleShape top;
    sf::RectangleShape bottom;
    sf::RectangleShape left;
    sf::RectangleShape right;
    int lineStrength = 1;

    top.setPosition(position.x, position.y);
    top.setSize(sf::Vector2f(width, lineStrength));

    bottom.setPosition(position.x, position.y + height - lineStrength);
    bottom.setSize(sf::Vector2f(width, lineStrength));

    left.setPosition(position.x, position.y);
    left.setSize(sf::Vector2f(lineStrength, height));

    right.setPosition(position.x + width - lineStrength, position.y);
    right.setSize(sf::Vector2f(lineStrength, height));

    window.draw(top);
    window.draw(bottom);
    window.draw(left);
    window.draw(right);
}

bool Hitbox::collision(Hitbox other)
{
    // collision check
    return position.y + height >= other.getPosition().y &&
           position.y <= other.getPosition().y + other.getHeight() &&
           position.x + width >= other.getPosition().x &&
           position.x <= other.getPosition().x + other.getWidth();
}

Direction Hitbox::collisionDirection(Hitbox other)
{
    // Box data
    sf::Vector2f thisPos = getPosition();
    float thisWidth = getWidth();
    float thisHeight = getHeight();

    // Box data
    sf::Vector2f otherPos = other.getPosition();
    float otherWidth = other.getWidth();
    float otherHeight = other.getHeight();

    // Directions
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;

    // Direction up
    if ((thisPos.y < otherPos.y))
    {
        up = true;
    }

    // Direction down
    if (thisPos.y + thisHeight > otherPos.y + otherHeight)
    {
        down = true;
    }

    // Direction left
    if (thisPos.x < otherPos.x)
    {
        left = true;
    }

    // Direction right
    if (thisPos.x + thisWidth > otherPos.x + otherWidth)
    {
        right = true;
    }

    // If two directions, check the right one
    if (up && left)
    {
        // check for smaller distance and handle accordingly
        if ((thisPos.y + thisHeight - otherPos.y) < (thisPos.x + thisWidth - otherPos.x))
        {
            left = false;
        }
        else
        {
            up = false;
        }
    }
    else if (up && right)
    {
        if ((thisPos.y + thisHeight - otherPos.y) < (otherPos.x + otherWidth - thisPos.x))
        {
            right = false;
        }
        else
        {
            up = false;
        }
    }
    else if (down && left)
    {
        if ((otherPos.y + otherHeight - thisPos.y) < (thisPos.x + thisWidth - otherPos.x))
        {
            left = false;
        }
        else
        {
            down = false;
        }
    }
    else if (down && right)
    {
        if ((otherPos.y + otherHeight - thisPos.y) < (otherPos.x + otherWidth - thisPos.x))
        {
            right = false;
        }
        else
        {
            down = false;
        }
    }

    // Handle the direction
    if (up)
    {
        return Direction::UP;
    }
    else if (down)
    {
        return Direction::DOWN;
    }
    else if (left)
    {
        return Direction::LEFT;
    }
    else if (right)
    {
        return Direction::RIGHT;
    }

    return Direction::NOWHERE;
}

Direction Hitbox::fullCollisionCheck(Hitbox other)
{
    if (collision(other))
    {
        return collisionDirection(other);
    }
    return Direction::NOWHERE;
}
