#pragma once
#include <SFML/Graphics.hpp>

#include "player.hpp"
#include "character.hpp"
#include "animator.hpp"

class Enemy : public Character
{
public:
    Enemy(std::vector<sf::Texture> &textures, sf::Vector2f position, sf::Vector2f size, int maxHealth, int minDamage, int maxDamage, int speed, int experience, Player *player);
    ~Enemy();

    bool operator==(const Enemy &other) const;

    void tick();
    void move();
    void draw(sf::RenderWindow &window);

    void drawHealthbar(sf::RenderWindow &window);
    sf::Sprite getBody();
    static int getNextId();
    int getId();

    void loadTextures(std::vector<sf::Texture> &textures);
    void setTexturePosition(int posX, int posY, int width, int height);
    void animationHandler();
    void updateAnimation();
    void mirrorTexture();
    void setInactive();
    bool isActive();
    int getExp();

private:
    sf::Sprite enemySprite;
    int currentFrame;
    float enemyDeltaTime;
    int experience;

    bool active;

    Animator animator;
    bool mirrored;

    Player *player;
    int id;
    static int nextId;
};
