#include "particle.hpp"

Particle::Particle(sf::Vector2f startPosition, sf::Vector2f targetPosition, sf::Time lifeSpan, int speed, int size, sf::Color color)
{
    body = sf::CircleShape(size);
    body.setPosition(startPosition);
    body.setFillColor(color);

    setAngle(targetPosition);
    this->speed = speed;
    this->lifeSpan = lifeSpan;
    visible = true;
}

Particle::Particle() {}

Particle::~Particle() {}

void Particle::tick()
{
    deltaTime = clock.getElapsedTime().asSeconds();
    updateLifeTime();
    fading();
    move();
}

void Particle::draw(sf::RenderWindow &window)
{
    window.draw(body);
}

void Particle::move()
{
    float movement = speed * deltaTime;
    setPosition(sf::Vector2f(getPosition().x + (movement * angle.x), getPosition().y + (movement * angle.y)));
}

void Particle::fading()
{
    sf::Color color = body.getFillColor();
    float visibility = 1 - (lifeTime.asSeconds() / lifeSpan.asSeconds());
    int alpha = 255 * visibility;
    color.a = alpha;
    body.setFillColor(color);
    if (alpha <= 1)
    {
        visible = false;
    }
}

void Particle::setAngle(sf::Vector2f targetPosition)
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

void Particle::setPosition(sf::Vector2f position)
{
    body.setPosition(position);
}

void Particle::updateLifeTime()
{
    lifeTime += sf::seconds(deltaTime);
}

bool Particle::isVisible()
{
    return visible;
}

sf::Vector2f Particle::getPosition()
{
    return body.getPosition();
}