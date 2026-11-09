#pragma once
#include <SFML/Graphics.hpp>
#include "direction.hpp"
#include "character.hpp"
#include "projectile.hpp"
#include "animator.hpp"

class Player : public Character
{
public:
    Player();
    Player(sf::Vector2f position, int maxHealth, int minDamage, int maxDamage, int speed);
    ~Player();

    void tick();
    void move();

    void setMoveDirection(Direction direction);
    void stopMoveDirection(Direction direction);

    void setBounds(int vertical, int horizontal);
    void loadPlayerTextures();
    void setTexturePosition();
    void animationHandler();
    void updateAnimation();
    void mirrorTexture();

    void checkCharacterState();
    void handleCharacterState();

    void addExperience(int exp);

    int getExperienceStart();
    int getExperience();
    int getExperienceNeeded();
    int getLevel();

    void setMinDamage(int damage);
    void setMaxDamage(int damage);
    void setSpeed(int speed);
    void setMaxHealth(int health);
    void setAttackSpeed(sf::Time attackSpeed);

    void draw(sf::RenderWindow &window);

private:
    sf::Sprite playerSprite;
    int currentFrame;
    float playerDeltaTime;

    int experienceStart;
    int experience;
    int experienceNeeded;
    int level;

    Animator animator;
    int characterState;
    bool mirrored;

    bool up;
    bool down;
    bool left;
    bool right;

    int verticalBorder;
    int horizontalBorder;
};