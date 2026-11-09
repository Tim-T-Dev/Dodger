#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "pausableClock.hpp"

class Particle
{
public:
    Particle(sf::Vector2f startPosition, sf::Vector2f targetPosition, sf::Time lifeSpan, int speed, int size, sf::Color color);
    Particle();
    ~Particle();

    void tick();
    void draw(sf::RenderWindow &window);
    void move();
    void fading();
    void setAngle(sf::Vector2f targetPosition);
    void setPosition(sf::Vector2f position);
    void updateLifeTime();
    bool isVisible();
    sf::Vector2f getPosition();

private:
    sf::CircleShape body;

    PausableClock clock;
    sf::Time lifeTime;
    sf::Time lifeSpan;
    float deltaTime;

    int speed;
    sf::Vector2f angle;

    bool visible;
};