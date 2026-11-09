#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <iostream>

#include "player.hpp"
#include "randomNumber.hpp"
#include "enemy.hpp"
#include "projectile.hpp"
#include "particle.hpp"
#include "direction.hpp"
#include "gameState.hpp"

class Game
{
public:
    Game(int width, int height);
    ~Game();

    void start();

private:
    sf::RenderWindow window;

    // MAIN CONSTRUCTION
    void events();
    void logic();
    void render();
    sf::Vector2u windowSize;
    GameState gameState;
    void initialSettings();

    // INGAME
    void handleGameEvents(sf::Event event);
    void gameLogic();
    void renderGame();

    // PLAYER
    Player player;
    void drawPlayerHealthBar();

    // EVENTS
    void closeEvent(sf::Event event);
    void resizeEvent(sf::Event event);
    void playerSpawnProjectileEvent(sf::Event event);
    void playerMoveEvent(sf::Event event);
    void playerStopEvent(sf::Event event);

    // CURSOR
    sf::Texture cursorTexture;
    sf::Sprite cursorSprite;
    void loadMouseCursor();
    void drawMouseCursor();

    // BACKGROUND
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    void loadBackground();
    void drawBackground();

    // ENEMIES
    std::vector<Enemy> enemies;
    sf::Clock spawnTimer;
    std::vector<sf::Texture> smallCultistTextures;
    std::vector<sf::Texture> bigCultistTextures;
    int inactiveCount;
    void deleteInactiveEnemies();
    void killEnemyCheck(Enemy &enemy);
    void spawnEnemies();
    void tickEnemies();
    void drawEnemies();
    void loadEnemyTextures();
    std::vector<sf::Texture> getEnemyTexturesFromPaths(std::vector<std::string> paths);
    sf::Vector2f randomSpawnLocation(int minRange, int maxRange);

    // PROJECTILES
    std::vector<Projectile> playerProjectiles;
    int projectileSpeed;
    bool projectileLeft;
    bool projectileRight;
    bool projectileUp;
    bool projectileDown;
    bool projectileMouse;
    void initialProjectileSettings();
    void checkIfProjectileSpawns();
    void addPlayerProjectile(sf::Vector2f targetPosition);
    void tickProjectiles();
    void drawProjectiles();

    // EFFECTS
    std::vector<Particle> killParticles;
    void drawParticles();
    void killEffect(Enemy enemy);
    void tickParticles();

    // Score
    int score;
    void drawScore();

    // COLLISIONS
    void handleAllCollsions();
    void playerEnemyCollision();
    void projectileCollision();

    // MAIN MENU
    sf::Texture mainMenuTexture1;
    sf::Texture mainMenuTexture2;
    sf::Texture mainMenuTexture3;
    sf::Texture mainMenuTexture4;
    sf::Sprite mainMenuSprite;
    int currentMenuTexture;

    void loadMainMenuTexture();
    void setMainMenuTexture(sf::Texture &texture);
    void mainMenuHover();

    void renderMainMenu();
    void handleMainMenuEvents(sf::Event event);
    void mainMenuLogic();

    // SETTINGS MENU
    sf::Texture settingsTexture1;
    sf::Texture settingsTexture2;
    sf::Sprite settingsSprite;
    int currentSettingsTexture;

    void loadSettingsTexture();
    void setSettingsTexture(sf::Texture &texture);
    void settingsHover();

    void renderSettings();
    void handleSettingsEvents(sf::Event event);
    void settingsLogic();

    // PROGRESSION
    sf::Texture levelUpMenuTexture;
    std::vector<sf::Texture> playerSpeedUpgradeTextures;
    std::vector<sf::Texture> attackSpeedUpgradeTextures;
    std::vector<sf::Texture> shotSpeedUpgradeTextures;
    std::vector<sf::Texture> damageUpgradeTextures;
    std::vector<sf::Texture> healthUpgradeTextures;
    std::vector<sf::Texture> allStatsUpgradeTextures;

    sf::Sprite levelUpMenuSprite;
    std::vector<sf::Sprite> upgradeIcons;
    std::vector<std::string> descriptions;

    std::vector<int> selectedUpgrades;
    std::vector<int> selectedUpgradeLevels;

    int levelUpPoints;
    int currentLevel;
    bool upgradesSet;

    void loadLevelUpTextures();
    void drawExperienceBar();
    void drawLevelUpDisplay();
    void checkLevelUp();
    void openLevelUpMenu(sf::Event event);

    void setLevelUpTextures();
    void setUpgrades();
    void setUpgradeDescriptions();

    void renderLevelUpMenu();
    void handleLevelUpEvents(sf::Event event);
    void levelUpLogic();

    void selectUpgrade(sf::Event event);
    void finishUpgrade(int decision);
    void resetUpgradeScreen();

    // PAUSE MENU
    bool isPause;
    void openClosePauseMenuEvent(sf::Event event);
    void drawPauseMenu();
    void pause();
    void resume();

    // DEFEAT MENU
    bool gameOver;
    void drawDefeatMenu();
    void defeat();
    void reset();
    void resetEvent(sf::Event event);

    // Audio
    sf::Music gameMusic;
    int volume;
    void loadMusic();
    void playMusic();

    // Font
    sf::Font font;
    void loadFont();

    // other
    void shuffle(std::vector<int> &list);
    void mousePositionEvent(sf::Event event);
};