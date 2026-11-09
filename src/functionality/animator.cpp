// Animator.cpp
#include "Animator.hpp"

Animator::Animator() : currentFrame(0), animationSpeed(2), frameChange(0) {}

bool Animator::loadTextures(const std::vector<std::string> &filePaths)
{
    playerTextures.clear();
    for (const auto &filePath : filePaths)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(filePath))
        {
            return false;
        }
        playerTextures.push_back(texture);
    }
    return true;
}

void Animator::setTextures(const std::vector<sf::Texture> &textures)
{
    for (const auto &texture : textures)
    {
        playerTextures.push_back(texture);
    }
}

void Animator::updateAnimation(float deltaTime)
{
    frameChange += animationSpeed * deltaTime;

    if (static_cast<int>(frameChange) >= playerTextures.size())
    {
        frameChange -= static_cast<float>(playerTextures.size());
    }
    currentFrame = static_cast<std::size_t>(frameChange);
}

const sf::Texture &Animator::getCurrentTexture() const
{
    return playerTextures[currentFrame];
}

const sf::Texture &Animator::getTexture(int index) const
{
    return playerTextures[index];
}

void Animator::setSpeed(float speed)
{
    animationSpeed = speed;
}
