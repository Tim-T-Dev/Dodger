// Animator.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Animator
{
public:
    Animator();
    bool loadTextures(const std::vector<std::string> &filePaths);
    void setTextures(const std::vector<sf::Texture> &textures);
    void updateAnimation(float deltaTime);
    const sf::Texture &getCurrentTexture() const;
    const sf::Texture &getTexture(int index) const;
    void setSpeed(float speed);

private:
    std::vector<sf::Texture> playerTextures;
    std::size_t currentFrame;
    float frameChange;
    float animationSpeed;
};