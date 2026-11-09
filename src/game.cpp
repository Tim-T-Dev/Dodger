#include "game.hpp"

Game::Game(int width = 1280, int height = 720)
{
    window.create(sf::VideoMode(width, height), "Dodger", sf::Style::Default, sf::ContextSettings(0, 0, 8));
    windowSize = sf::Vector2u(width, height);
    window.setFramerateLimit(120);

    // Load resources
    loadMouseCursor();
    loadBackground();
    loadEnemyTextures();
    loadMainMenuTexture();
    loadSettingsTexture();
    loadFont();
    loadLevelUpTextures();
    loadMusic();

    // Other
    initialProjectileSettings();
    gameState = GameState::MainMenu;
}

Game::~Game() {};

void Game::start()
{
    while (window.isOpen())
    {
        events();
        logic();
        render();
    }
}

void Game::initialSettings()
{
    player = Player(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2), 200, 10, 15, 120);
    score = 0;
    gameOver = false;
    isPause = false;
    player.setBounds(window.getSize().y, window.getSize().x);
    spawnTimer.restart();
    inactiveCount = 0;
    projectileSpeed = 400;

    // Leveling stuff;
    levelUpPoints = 0;
    currentLevel = 0;
    upgradesSet = false;
}

void Game::handleGameEvents(sf::Event event)
{
    playerMoveEvent(event);
    playerStopEvent(event);
    playerSpawnProjectileEvent(event);
    openClosePauseMenuEvent(event);
    resetEvent(event);
    openLevelUpMenu(event);
}

void Game::gameLogic()
{
    if (!isPause)
    {
        playMusic();
        spawnEnemies();
        player.tick();
        tickEnemies();
        tickProjectiles();
        tickParticles();
        handleAllCollsions();
    }
}

void Game::renderGame()
{
    // Background
    drawBackground();

    // GameObjects
    drawProjectiles();
    drawEnemies();
    drawParticles();
    player.draw(window);

    // HUD
    drawPlayerHealthBar();
    drawExperienceBar();
    drawScore();
    drawLevelUpDisplay();

    drawPauseMenu();
    drawDefeatMenu();
}

// Game logic
void Game::logic()
{
    if (gameState == GameState::MainMenu)
    {
        mainMenuLogic();
    }
    else if (gameState == GameState::InGame)
    {
        gameLogic();
    }
    else if (gameState == GameState::LevelUpMenu)
    {
        levelUpLogic();
    }
    else if (gameState == GameState::Settings)
    {
        settingsLogic();
    }
}

void Game::render()
{
    window.clear();

    if (gameState == GameState::MainMenu)
    {
        renderMainMenu();
    }
    else if (gameState == GameState::InGame)
    {
        renderGame();
    }
    else if (gameState == GameState::LevelUpMenu)
    {
        renderLevelUpMenu();
    }
    else if (gameState == GameState::Settings)
    {
        renderSettings();
    }
    drawMouseCursor();
    window.display();
}

void Game::events()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        closeEvent(event);
        resizeEvent(event);

        if (gameState == GameState::MainMenu)
        {
            handleMainMenuEvents(event);
        }
        else if (gameState == GameState::InGame)
        {
            handleGameEvents(event);
        }
        else if (gameState == GameState::LevelUpMenu)
        {
            handleLevelUpEvents(event);
        }
        else if (gameState == GameState::Settings)
        {
            handleSettingsEvents(event);
        }
    }
}

void Game::closeEvent(sf::Event event)
{
    if (event.type == sf::Event::Closed)
    {
        window.close();
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
    {
        window.close();
    }
}

void Game::resizeEvent(sf::Event event)
{
    if (event.type == sf::Event::Resized)
    {
        window.setSize(windowSize);
    }
}

void Game::playerMoveEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::W)
        {
            player.setMoveDirection(Direction::UP);
        }
        else if (event.key.code == sf::Keyboard::A)
        {
            player.setMoveDirection(Direction::LEFT);
        }
        else if (event.key.code == sf::Keyboard::S)
        {
            player.setMoveDirection(Direction::DOWN);
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            player.setMoveDirection(Direction::RIGHT);
        }
    }
}

void Game::playerStopEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::W)
        {
            player.stopMoveDirection(Direction::UP);
        }
        else if (event.key.code == sf::Keyboard::A)
        {
            player.stopMoveDirection(Direction::LEFT);
        }
        else if (event.key.code == sf::Keyboard::S)
        {
            player.stopMoveDirection(Direction::DOWN);
        }
        else if (event.key.code == sf::Keyboard::D)
        {
            player.stopMoveDirection(Direction::RIGHT);
        }
    }
}

void Game::playerSpawnProjectileEvent(sf::Event event)
{

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (!projectileMouse)
        {
            projectileMouse = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        projectileMouse = false;
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Up && !projectileUp)
        {
            projectileUp = true;
        }
        else if (event.key.code == sf::Keyboard::Left && !projectileLeft)
        {
            projectileLeft = true;
        }
        else if (event.key.code == sf::Keyboard::Down && !projectileDown)
        {
            projectileDown = true;
        }
        else if (event.key.code == sf::Keyboard::Right && !projectileRight)
        {
            projectileRight = true;
        }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
        if (event.key.code == sf::Keyboard::Up)
        {
            projectileUp = false;
        }
        else if (event.key.code == sf::Keyboard::Left)
        {
            projectileLeft = false;
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
            projectileDown = false;
        }
        else if (event.key.code == sf::Keyboard::Right)
        {
            projectileRight = false;
        }
    }
}

void Game::loadMouseCursor()
{
    if (!cursorTexture.loadFromFile("resources/cursor2.png"))
    {
        return;
    }

    // set new mouse cursor
    window.setMouseCursorVisible(false);
    cursorSprite.setTexture(cursorTexture);
    float scaleFactor = 0.2f;
    cursorSprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));
}

void Game::drawMouseCursor()
{
    // draw mouse cursor
    cursorSprite.setPosition(sf::Mouse::getPosition(window).x - 7, sf::Mouse::getPosition(window).y - 7);
    window.draw(cursorSprite);
}

void Game::loadBackground()
{
    if (!backgroundTexture.loadFromFile("resources/background.png"))
    {
        return;
    }

    // set background
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / backgroundTexture.getSize().x,
        static_cast<float>(windowSize.y) / backgroundTexture.getSize().y);
}

void Game::drawBackground()
{
    window.draw(backgroundSprite);
}

void Game::deleteInactiveEnemies()
{
    std::vector<Enemy> newList;
    for (auto &enemy : enemies)
    {
        if (enemy.isActive())
        {
            newList.push_back(enemy);
        }
    }
    enemies = newList;
    inactiveCount = 0;
}

void Game::killEnemyCheck(Enemy &enemy)
{
    if (enemy.getHealth() <= 0)
    {
        killEffect(enemy);
        enemy.setInactive();
        inactiveCount++;
        score += enemy.getExp() / 3;

        // leveling
        player.addExperience(enemy.getExp());
        checkLevelUp();

        if (inactiveCount >= 200)
        {
            deleteInactiveEnemies();
        }
    }
}

void Game::spawnEnemies()
{
    sf::Time spawnRate = sf::seconds(7);
    if (spawnTimer.getElapsedTime() >= spawnRate)
    {
        spawnTimer.restart();
        int spawnAmount = 3;
        sf::Vector2f enemySize(40, 40);
        int maxRange = 70;
        int minRange = 40;

        int smallCultistWeight = 100;
        int bigCultistWeight = 0;
        float difficultyIncrease = 1;

        // Scoretweaks
        if (score > 100)
        {
            spawnAmount += 1;
        }
        if (score > 300)
        {
            bigCultistWeight += 5;
            spawnAmount -= 1;
            spawnAmount += RandomNumber::getRandomInt(1, 2);
        }
        if (score > 500)
        {
            spawnAmount += RandomNumber::getRandomInt(1, 2);
        }
        if (score > 800)
        {
            spawnAmount += RandomNumber::getRandomInt(2, 3);
        }
        if (score > 1000)
        {
            spawnAmount -= 3;
            bigCultistWeight += 3;
            difficultyIncrease += 0.2f;
        }
        if (score > 1200)
        {
            spawnAmount += 1;
            bigCultistWeight += 5;
            spawnRate = sf::seconds(6);

            // scale to infinite
            spawnAmount += (score - 1200) / 300;

            for (int i = 0; i < ((score - 1200) / 500) * 0.2f; i++)
            {
                difficultyIncrease *= 1.1f;
            }
        }

        int weightSum = smallCultistWeight + bigCultistWeight;

        for (int i = 0; i < spawnAmount; i++)
        {
            sf::Vector2f position = randomSpawnLocation(minRange, maxRange);
            int num = RandomNumber::getRandomInt(0, weightSum);
            int currentWeight = smallCultistWeight;

            // Small Cultist
            if (num >= 0 && num <= currentWeight)
            {
                int maxHealth = 40 * difficultyIncrease;
                int minDamage = 20 * difficultyIncrease;
                int maxDamage = 30 * difficultyIncrease;
                int speed = 50 * difficultyIncrease;
                int experience = 60 * difficultyIncrease;

                if (speed > 150)
                    speed = 150;

                Enemy enemy(smallCultistTextures, position, enemySize, maxHealth, minDamage, maxDamage, speed, experience, &player);
                enemy.setTexturePosition(-17, -15, -10, -10);
                enemies.push_back(enemy);
                continue;
            }

            // Update Weights
            int lastWeight = currentWeight;
            currentWeight += bigCultistWeight;

            // Big Cultist
            if (num > lastWeight && num <= currentWeight)
            {
                int maxHealth = 150 * difficultyIncrease;
                int minDamage = 35 * difficultyIncrease;
                int maxDamage = 50 * difficultyIncrease;
                int speed = 35 * difficultyIncrease;
                int experience = 300 * difficultyIncrease;

                Enemy enemy(bigCultistTextures, position, enemySize, maxHealth, minDamage, maxDamage, speed, experience, &player);
                enemy.setTexturePosition(-14, -2, -2, 15);
                enemies.push_back(enemy);
                continue;
            }

            // Update Weights
            lastWeight = currentWeight;
            currentWeight += 0;
        }
    }
}

void Game::tickEnemies()
{
    for (auto &enemy : enemies)
    {
        enemy.tick();
    }
}

void Game::drawEnemies()
{
    for (auto &enemy : enemies)
    {
        enemy.draw(window);
    }
}

void Game::loadEnemyTextures()
{
    std::vector<std::string> texturePathsSmallCultist;
    texturePathsSmallCultist.push_back("resources/enemys/smallCultist/cultist1.png");
    texturePathsSmallCultist.push_back("resources/enemys/smallCultist/cultist2.png");
    smallCultistTextures = getEnemyTexturesFromPaths(texturePathsSmallCultist);

    std::vector<std::string> texturePathsBigCultist;
    texturePathsBigCultist.push_back("resources/enemys/bigCultist/cultistBig1.png");
    texturePathsBigCultist.push_back("resources/enemys/bigCultist/cultistBig2.png");
    texturePathsBigCultist.push_back("resources/enemys/bigCultist/cultistBig3.png");
    texturePathsBigCultist.push_back("resources/enemys/bigCultist/cultistBig4.png");
    bigCultistTextures = getEnemyTexturesFromPaths(texturePathsBigCultist);
}

std::vector<sf::Texture> Game::getEnemyTexturesFromPaths(std::vector<std::string> paths)
{
    std::vector<sf::Texture> textures;
    for (const auto &path : paths)
    {
        sf::Texture texture;
        if (!texture.loadFromFile(path))
        {
            exit(-1);
        }
        textures.push_back(texture);
    }
    return textures;
}

sf::Vector2f Game::randomSpawnLocation(int minRange, int maxRange)
{
    sf::Vector2f position;
    int switcher = RandomNumber::getRandomInt(1, 4);
    switch (switcher)
    {
    case 1: // UP
        position.x = RandomNumber::getRandomInt(-maxRange, window.getSize().x + maxRange);
        position.y = RandomNumber::getRandomInt(-maxRange, -minRange);
        break;
    case 2: // DOWN
        position.x = RandomNumber::getRandomInt(-maxRange, window.getSize().x + maxRange);
        position.y = RandomNumber::getRandomInt(window.getSize().y + minRange, window.getSize().y + maxRange);
        break;
    case 3: // LEFT
        position.x = RandomNumber::getRandomInt(-maxRange, -minRange);
        position.y = RandomNumber::getRandomInt(0, window.getSize().y);
        break;
    case 4: // RIGHT
        position.x = RandomNumber::getRandomInt(window.getSize().x + minRange, window.getSize().x + maxRange);
        position.y = RandomNumber::getRandomInt(0, window.getSize().y);
        break;
    default:
        break;
    }
    return position;
}

void Game::initialProjectileSettings()
{
    projectileDown = false;
    projectileUp = false;
    projectileLeft = false;
    projectileRight = false;
    projectileMouse = false;
}

void Game::checkIfProjectileSpawns()
{
    if (player.canAttack())
    {
        bool attack = false;
        sf::Vector2f target(player.getPosition().x, player.getPosition().y);

        if (projectileMouse)
        {
            target = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
            addPlayerProjectile(target);
            return;
        }

        if (projectileUp && !projectileDown)
        {
            target.y -= 1000000;
            attack = true;
        }

        if (projectileLeft && !projectileRight)
        {
            target.x -= 1000000;
            attack = true;
        }

        if (projectileDown && !projectileUp)
        {
            target.y += 1000000;
            attack = true;
        }

        if (projectileRight && !projectileLeft)
        {
            target.x += 1000000;
            attack = true;
        }

        if (attack)
        {
            addPlayerProjectile(target);
        }
    }
}

void Game::addPlayerProjectile(sf::Vector2f targetPosition)
{
    sf::Vector2f startPosition = player.getPosition();
    startPosition.x += (player.getSize().x / 2) - 5;
    startPosition.y += (player.getSize().y / 2) - 5;

    playerProjectiles.push_back(Projectile(startPosition, targetPosition, player.attack(), projectileSpeed, 6, sf::Color(250, 250, 255, 255)));
}

void Game::tickProjectiles()
{
    checkIfProjectileSpawns();

    for (auto it = playerProjectiles.begin(); it != playerProjectiles.end();)
    {
        it->tick();

        // check if projectile is outside the window
        if (it->getPosition().x < 0 - 100 ||
            it->getPosition().y < 0 - 100 ||
            it->getPosition().x > window.getSize().x + 100 ||
            it->getPosition().y > window.getSize().y + 100)
        {
            // remove projectile from list
            it = playerProjectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::drawProjectiles()
{
    for (auto &projectile : playerProjectiles)
    {
        window.draw(projectile.getBody());
    }
}

void Game::drawParticles()
{
    for (auto &particle : killParticles)
    {
        particle.draw(window);
    }
}

void Game::killEffect(Enemy enemy)
{
    sf::Time lifeSpan = sf::seconds(1.5f);
    int particleAmount = 7;
    int particleSize = 3;
    sf::Color particleColor(240, 240, 255);

    // Start
    sf::Vector2f start(
        enemy.getPosition().x + (enemy.getSize().x / 2) - particleSize,
        enemy.getPosition().y + (enemy.getSize().y / 2) - particleSize);

    for (int i = 0; i < particleAmount; i++)
    {
        // Speed
        int particleSpeed = RandomNumber::getRandomInt(20, 50);

        // Target
        sf::Vector2f target(
            RandomNumber::getRandomInt(enemy.getPosition().x, enemy.getPosition().x + enemy.getSize().x),
            RandomNumber::getRandomInt(enemy.getPosition().y, enemy.getPosition().y + enemy.getSize().y));

        Particle particle(start, target, lifeSpan, particleSpeed, particleSize, particleColor);
        killParticles.push_back(particle);
    }
}

void Game::tickParticles()
{
    for (auto it = killParticles.begin(); it != killParticles.end();)
    {
        it->tick();
        if (!(it->isVisible()))
        {
            it = killParticles.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Game::drawScore()
{
    sf::Text scoreText("Score: " + std::to_string(score), font, 30);
    scoreText.setPosition(10, 0);
    window.draw(scoreText);
}

void Game::handleAllCollsions()
{
    // Player with enemies
    playerEnemyCollision();

    // Player projectiles
    projectileCollision();
}

void Game::playerEnemyCollision()
{
    // go over all enemies
    for (auto &enemy : enemies)
    {
        if (!player.getHitbox().collision(enemy.getHitbox()))
        {
            continue;
        }
        // Player takes damage
        player.takeDamage(enemy.attack());
        if (player.getHealth() <= 0)
        {
            defeat();
        }
    }
}

void Game::projectileCollision()
{
    for (auto it = playerProjectiles.begin(); it != playerProjectiles.end();)
    {
        bool removeProjectile = false;

        // Collision with enemies
        for (auto &enemy : enemies)
        {
            if (it->getHitbox().collision(enemy.getHitbox()))
            {
                removeProjectile = true;
                enemy.takeDamage(it->getDamage());
                killEnemyCheck(enemy);
                break;
            }
        }

        // Remove Projectile
        if (removeProjectile)
        {
            it = playerProjectiles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::loadMainMenuTexture()
{
    if (!mainMenuTexture1.loadFromFile("resources/mainMenu/MainMenu1.png"))
    {
        return;
    }

    if (!mainMenuTexture2.loadFromFile("resources/mainMenu/MainMenu2.png"))
    {
        return;
    }

    if (!mainMenuTexture3.loadFromFile("resources/mainMenu/MainMenu3.png"))
    {
        return;
    }

    if (!mainMenuTexture4.loadFromFile("resources/mainMenu/MainMenu4.png"))
    {
        return;
    }
}

void Game::setMainMenuTexture(sf::Texture &texture)
{
    // set menu background
    sf::Vector2u windowSize = window.getSize();
    mainMenuSprite.setTexture(texture);
    mainMenuSprite.setScale(
        static_cast<float>(windowSize.x) / texture.getSize().x,
        static_cast<float>(windowSize.y) / texture.getSize().y);
}

void Game::renderMainMenu()
{
    window.draw(mainMenuSprite);
}

void Game::handleMainMenuEvents(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        int x = sf::Mouse().getPosition(window).x;
        int y = sf::Mouse().getPosition(window).y;

        // Button 1 x:41  y:288  | x:354  y:359
        // Button 2 x:41  y:397  | x:354  y:466
        // Button 3 x:41  y:504  | x:354  y:575

        // Button 1 clicked
        if ((x >= 41 && x <= 354) && (y >= 288 && y <= 359))
        {
            initialSettings();
            gameState = GameState::InGame;
        }
        // Button 2 clicked
        else if ((x >= 41 && x <= 354) && (y >= 397 && y <= 466))
        {
            gameState = GameState::Settings;
            currentSettingsTexture = 0;
        }
        // Button 3 clicked
        else if ((x >= 41 && x <= 354) && (y >= 504 && y <= 575))
        {
            window.close();
        }
    }
}

void Game::mainMenuLogic()
{
    mainMenuHover();
}

void Game::loadSettingsTexture()
{
    if (!settingsTexture1.loadFromFile("resources/settings/settingsMenu1.png"))
    {
        return;
    }

    if (!settingsTexture2.loadFromFile("resources/settings/settingsMenu2.png"))
    {
        return;
    }
}

void Game::setSettingsTexture(sf::Texture &texture)
{
    // set background
    sf::Vector2u windowSize = window.getSize();
    settingsSprite.setTexture(texture);
    settingsSprite.setScale(
        static_cast<float>(windowSize.x) / texture.getSize().x,
        static_cast<float>(windowSize.y) / texture.getSize().y);
}

void Game::settingsHover()
{
    int x = sf::Mouse().getPosition(window).x;
    int y = sf::Mouse().getPosition(window).y;

    if ((x >= 50 && x <= 360) && (y >= 610 && y <= 680))
    {
        if (currentSettingsTexture != 2)
        {
            setSettingsTexture(settingsTexture2);
            currentSettingsTexture = 2;
        }
    }
    else if (currentSettingsTexture != 1)
    {
        setSettingsTexture(settingsTexture1);
        currentSettingsTexture = 1;
    }
}

void Game::renderSettings()
{
    std::string titleText = ("\nCONTROLS\n");
    std::string attackText = ("\nAttack\n- Left click to attack.\n- Alternative: Arrow keys\n");
    std::string movementText = ("\nMovement\n- W A S D to move your character\n");
    std::string menuText = ("\nMenus \n- Esc to exit the program\n- P to pause the game\n- Tab to level up\n- R to restart after defeat\n");
    std::string text = titleText + attackText + movementText + menuText;
    sf::Text settingsText(text, font, 32);
    settingsText.setPosition(777, 120);
    settingsText.setFillColor(sf::Color(170, 210, 230, 255));

    window.draw(settingsSprite);
    window.draw(settingsText);
}

void Game::handleSettingsEvents(sf::Event event)
{

    if (event.type == sf::Event::MouseButtonPressed)
    {
        int x = sf::Mouse().getPosition(window).x;
        int y = sf::Mouse().getPosition(window).y;

        // Button clicked
        if ((x >= 50 && x <= 360) && (y >= 610 && y <= 680))
        {
            gameState = GameState::MainMenu;
        }
    }
}

void Game::settingsLogic()
{
    settingsHover();
}

void Game::mainMenuHover()
{
    int x = sf::Mouse().getPosition(window).x;
    int y = sf::Mouse().getPosition(window).y;

    if ((x >= 41 && x <= 354) && (y >= 288 && y <= 359))
    {
        if (currentMenuTexture != 2)
        {
            setMainMenuTexture(mainMenuTexture2);
            currentMenuTexture = 2;
        }
    }
    // Button 2 hovered
    else if ((x >= 41 && x <= 354) && (y >= 397 && y <= 466))
    {
        if (currentMenuTexture != 3)
        {
            setMainMenuTexture(mainMenuTexture3);
            currentMenuTexture = 3;
        }
    }
    // Button 3 hovered
    else if ((x >= 41 && x <= 354) && (y >= 504 && y <= 575))
    {
        if (currentMenuTexture != 4)
        {
            setMainMenuTexture(mainMenuTexture4);
            currentMenuTexture = 4;
        }
    }
    else if (currentMenuTexture != 1)
    {
        setMainMenuTexture(mainMenuTexture1);
        currentMenuTexture = 1;
    }
}

void Game::loadLevelUpTextures()
{
    if (!levelUpMenuTexture.loadFromFile("resources/levelUp/UpgradeScreen.png"))
    {
        return;
    }

    std::vector<std::string> paths;
    paths.push_back("resources/levelUp/AllStats/");
    paths.push_back("resources/levelUp/AttackSpeed/");
    paths.push_back("resources/levelUp/Damage/");
    paths.push_back("resources/levelUp/Health/");
    paths.push_back("resources/levelUp/PlayerSpeed/");
    paths.push_back("resources/levelUp/ShotSpeed/");

    std::vector<sf::Texture> upgradeTextures;
    sf::Texture texture;

    for (auto &path : paths)
    {
        if (!texture.loadFromFile(path + "level1.png"))
        {
            return;
        }
        upgradeTextures.push_back(texture);

        if (!texture.loadFromFile(path + "level2.png"))
        {
            return;
        }
        upgradeTextures.push_back(texture);

        if (!texture.loadFromFile(path + "level3.png"))
        {
            return;
        }
        upgradeTextures.push_back(texture);
    }

    allStatsUpgradeTextures.push_back(upgradeTextures.at(0));
    allStatsUpgradeTextures.push_back(upgradeTextures.at(1));
    allStatsUpgradeTextures.push_back(upgradeTextures.at(2));

    attackSpeedUpgradeTextures.push_back(upgradeTextures.at(3));
    attackSpeedUpgradeTextures.push_back(upgradeTextures.at(4));
    attackSpeedUpgradeTextures.push_back(upgradeTextures.at(5));

    damageUpgradeTextures.push_back(upgradeTextures.at(6));
    damageUpgradeTextures.push_back(upgradeTextures.at(7));
    damageUpgradeTextures.push_back(upgradeTextures.at(8));

    healthUpgradeTextures.push_back(upgradeTextures.at(9));
    healthUpgradeTextures.push_back(upgradeTextures.at(10));
    healthUpgradeTextures.push_back(upgradeTextures.at(11));

    playerSpeedUpgradeTextures.push_back(upgradeTextures.at(12));
    playerSpeedUpgradeTextures.push_back(upgradeTextures.at(13));
    playerSpeedUpgradeTextures.push_back(upgradeTextures.at(14));

    shotSpeedUpgradeTextures.push_back(upgradeTextures.at(15));
    shotSpeedUpgradeTextures.push_back(upgradeTextures.at(16));
    shotSpeedUpgradeTextures.push_back(upgradeTextures.at(17));
}

void Game::setLevelUpTextures()
{
    // set levelUp texture
    sf::Vector2u windowSize = window.getSize();
    levelUpMenuSprite.setTexture(levelUpMenuTexture);
    levelUpMenuSprite.setScale(
        static_cast<float>(windowSize.x) / levelUpMenuTexture.getSize().x,
        static_cast<float>(windowSize.y) / levelUpMenuTexture.getSize().y);

    sf::Sprite temp;
    for (int i = 0; i < 3; i++)
    {
        // Damage
        if (selectedUpgrades.at(i) == 1)
        {
            temp.setTexture(
                damageUpgradeTextures.at(selectedUpgradeLevels.at(i) - 1));
            upgradeIcons.push_back(temp);
        }

        // Player Speed
        else if (selectedUpgrades.at(i) == 2)
        {
            temp.setTexture(
                playerSpeedUpgradeTextures.at(selectedUpgradeLevels.at(i) - 1));
            upgradeIcons.push_back(temp);
        }

        // Attack Speed
        else if (selectedUpgrades.at(i) == 3)
        {
            temp.setTexture(
                attackSpeedUpgradeTextures.at(selectedUpgradeLevels.at(i) - 1));
            upgradeIcons.push_back(temp);
        }

        // Shot Speed
        else if (selectedUpgrades.at(i) == 4)
        {
            temp.setTexture(
                shotSpeedUpgradeTextures.at(selectedUpgradeLevels.at(i) - 1));
            upgradeIcons.push_back(temp);
        }

        // Health
        else if (selectedUpgrades.at(i) == 5)
        {
            temp.setTexture(
                healthUpgradeTextures.at(selectedUpgradeLevels.at(i) - 1));
            upgradeIcons.push_back(temp);
        }

        // All Stats
        else if (selectedUpgrades.at(i) == 6)
        {
            temp.setTexture(
                allStatsUpgradeTextures.at(selectedUpgradeLevels.at(i) - 1));
            upgradeIcons.push_back(temp);
        }
    }
    // setPositions
    upgradeIcons.at(0).setPosition(172, 144);
    upgradeIcons.at(1).setPosition(576, 172);
    upgradeIcons.at(2).setPosition(984, 144);
}

void Game::drawExperienceBar()
{
    int length = 800;
    int height = 10;
    int padding = 6;

    int xPos = (window.getSize().x / 2) - (length / 2);
    int yPos = 5;

    float currentExp = player.getExperienceNeeded() - player.getExperience();
    float neededExp = player.getExperienceNeeded() - player.getExperienceStart();

    float sizePercentage = 1 - (currentExp / neededExp);

    sf::Text currentLevel(std::to_string(player.getLevel()), font, 30);
    currentLevel.setPosition((window.getSize().x / 2) - 10, -3);
    currentLevel.setFillColor(sf::Color(230, 250, 255, 250));

    sf::RectangleShape expBackground(sf::Vector2f(length, height));
    sf::RectangleShape expBar(sf::Vector2f((length - padding) * sizePercentage, height - padding));

    expBackground.setPosition(xPos, yPos);
    expBar.setPosition(xPos + padding / 2, yPos + padding / 2);

    expBackground.setFillColor(sf::Color(20, 40, 60, 190));
    expBar.setFillColor(sf::Color(220, 255, 220, 150));

    window.draw(expBackground);
    window.draw(expBar);
    window.draw(currentLevel);
}

void Game::drawLevelUpDisplay()
{
    if (levelUpPoints == 0)
    {
        return;
    }

    sf::Vector2f size(97, 26);
    sf::Vector2i position(window.getSize().x - 120 - 10, 4);

    sf::Text text("Level Up!", font, 30);
    sf::RectangleShape displayBackground(size);

    displayBackground.setPosition(position.x - 4, position.y);
    text.setPosition(position.x, position.y - 5);

    displayBackground.setFillColor(sf::Color(20, 40, 60, 190));
    text.setFillColor(sf::Color(230, 250, 255, 250));

    window.draw(displayBackground);
    window.draw(text);
}

void Game::checkLevelUp()
{
    if (currentLevel == player.getLevel())
    {
        return;
    }

    levelUpPoints += player.getLevel() - currentLevel;
    currentLevel = player.getLevel();
}

void Game::openLevelUpMenu(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Tab)
        {
            if (levelUpPoints > 0 && !isPause)
            {
                gameState = GameState::LevelUpMenu;
            }
        }
    }
}

void Game::setUpgrades()
{
    int damageChance = 20;
    int playerSpeedChance = 20;
    int attackSpeedChance = 20;
    int shotSpeedChance = 20;
    int healthChance = 20;
    int allStatsUpgradeChance = 10;

    int level1Chance = 6;
    int level2Chance = 4;
    int level3Chance = 2;

    for (int i = 0; i < 3; i++)
    {
        int upgradeChancesSum = damageChance + playerSpeedChance + attackSpeedChance +
                                shotSpeedChance + healthChance + allStatsUpgradeChance;
        int upgradePick = RandomNumber::getRandomInt(0, upgradeChancesSum);

        // damage upgrade
        int currentStart = 0;
        int currentEnd = damageChance;
        if (upgradePick > currentStart && upgradePick <= currentEnd)
        {
            damageChance = 0;
            selectedUpgrades.push_back(1);
            continue;
        }

        // player speed upgrade
        currentStart = currentEnd;
        currentEnd += playerSpeedChance;
        if (upgradePick > currentStart && upgradePick <= currentEnd)
        {
            playerSpeedChance = 0;
            selectedUpgrades.push_back(2);
            continue;
        }

        // attack speed upgrade
        currentStart = currentEnd;
        currentEnd += attackSpeedChance;
        if (upgradePick > currentStart && upgradePick <= currentEnd)
        {
            attackSpeedChance = 0;
            selectedUpgrades.push_back(3);
            continue;
        }

        // shot speed upgrade
        currentStart = currentEnd;
        currentEnd += shotSpeedChance;
        if (upgradePick > currentStart && upgradePick <= currentEnd)
        {
            shotSpeedChance = 0;
            selectedUpgrades.push_back(4);
            continue;
        }

        // health upgrade
        currentStart = currentEnd;
        currentEnd += healthChance;
        if (upgradePick > currentStart && upgradePick <= currentEnd)
        {
            healthChance = 0;
            selectedUpgrades.push_back(5);
            continue;
        }

        // all stats upgrade
        currentStart = currentEnd;
        currentEnd += allStatsUpgradeChance;
        if (upgradePick > currentStart)
        {
            allStatsUpgradeChance = 0;
            selectedUpgrades.push_back(6);
            continue;
        }
        selectedUpgrades.push_back(6);
    }

    for (int i = 0; i < 3; i++)
    {
        int levelChancesSum = level1Chance + level2Chance + level3Chance;
        int levelPick = RandomNumber::getRandomInt(0, levelChancesSum);

        // upgrade level 1
        int currentStart = 0;
        int currentEnd = level1Chance;
        if (levelPick >= currentStart && levelPick < currentEnd)
        {
            selectedUpgradeLevels.push_back(1);
            continue;
        }

        // upgrade level 2
        currentStart = currentEnd;
        currentEnd += level2Chance;
        if (levelPick >= currentStart && levelPick < currentEnd)
        {
            selectedUpgradeLevels.push_back(2);
            continue;
        }

        // upgrade level 3
        currentStart = currentEnd;
        currentEnd += level3Chance;
        if (levelPick >= currentStart)
        {
            selectedUpgradeLevels.push_back(3);
            continue;
        }
        selectedUpgradeLevels.push_back(3);
    }
}

void Game::setUpgradeDescriptions()
{

    for (int i = 0; i < 3; i++)
    {
        int upgrade = selectedUpgrades.at(i);
        int level = selectedUpgradeLevels.at(i);
        std::string text = "";
        // Damage
        if (upgrade == 1)
        {
            if (level == 1)
            {
                text = "Moderate damage \nupgrade\n";
            }
            else if (level == 2)
            {
                text = "Great damage \nupgrade\n";
            }
            else if (level == 3)
            {
                text = "Colossal damage \nupgrade\n";
            }
        }
        // Player Speed
        else if (upgrade == 2)
        {
            if (level == 1)
            {
                text = "Moderate movement \nspeed upgrade\n";
            }
            else if (level == 2)
            {
                text = "Great movement \nspeed upgrade\n";
            }
            else if (level == 3)
            {
                text = "Colossal movement \nspeed upgrade\n";
            }
        }
        // Attack Speed
        else if (upgrade == 3)
        {
            if (level == 1)
            {
                text = "Moderate attack speed \nupgrade\n";
            }
            else if (level == 2)
            {
                text = "Great attack speed \nupgrade\n";
            }
            else if (level == 3)
            {
                text = "Colossal attack speed \nupgrade\n";
            }
        }
        // Shot Speed
        else if (upgrade == 4)
        {
            if (level == 1)
            {
                text = "Moderate projectile \nspeed upgrade\n";
            }
            else if (level == 2)
            {
                text = "Great projectile \nspeed upgrade\n";
            }
            else if (level == 3)
            {
                text = "Colossal projectile \nspeed upgrade\n";
            }
        }
        // Health
        else if (upgrade == 5)
        {
            if (level == 1)
            {
                text = "Moderate health \nupgrade\n\nRecover all HP\n";
            }
            else if (level == 2)
            {
                text = "Great health \nupgrade\n\nRecover all HP\n";
            }
            else if (level == 3)
            {
                text = "Colossal health \nupgrade\n\nRecover all HP\n";
            }
        }
        // All Stats
        else if (upgrade == 6)
        {
            if (level == 1)
            {
                text = "Moderate all stats \nupgrade\n";
            }
            else if (level == 2)
            {
                text = "Great all stats \nupgrade\n";
            }
            else if (level == 3)
            {
                text = "Colossal all stats \nupgrade\n\nRecover all HP\n";
            }
        }

        descriptions.push_back(text);
    }
}

void Game::renderLevelUpMenu()
{
    // game in background
    renderGame();
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().x));
    background.setFillColor(sf::Color(0, 0, 0, 150));

    sf::Text text1(descriptions.at(0), font, 30);
    sf::Text text2(descriptions.at(1), font, 30);
    sf::Text text3(descriptions.at(2), font, 30);
    text1.setPosition(120, 372);
    text2.setPosition(524, 408);
    text3.setPosition(928, 372);
    text1.setFillColor(sf::Color(170, 210, 230, 255));
    text2.setFillColor(sf::Color(170, 210, 230, 255));
    text3.setFillColor(sf::Color(170, 210, 230, 255));

    window.draw(background);
    window.draw(levelUpMenuSprite);
    window.draw(upgradeIcons.at(0));
    window.draw(upgradeIcons.at(1));
    window.draw(upgradeIcons.at(2));
    window.draw(text1);
    window.draw(text2);
    window.draw(text3);
}

void Game::handleLevelUpEvents(sf::Event event)
{
    selectUpgrade(event);
}

void Game::levelUpLogic()
{
    if (!upgradesSet)
    {
        setUpgrades();
        setUpgradeDescriptions();
        setLevelUpTextures();
        upgradesSet = true;
    }
}

void Game::selectUpgrade(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        int x = sf::Mouse::getPosition(window).x;
        int y = sf::Mouse::getPosition(window).y;

        if (x >= 74 && x <= 400 && y >= 110 && y <= 680)
        {
            finishUpgrade(1);
        }
        else if (x >= 480 && x <= 800 && y >= 120 && y <= 700)
        {
            finishUpgrade(2);
        }
        else if (x >= 885 && x <= 1200 && y >= 110 && y <= 680)
        {
            finishUpgrade(3);
        }
    }
}

void Game::finishUpgrade(int decision)
{
    int upgrade = selectedUpgrades.at(decision - 1);
    int level = selectedUpgradeLevels.at(decision - 1);

    // Damage
    if (upgrade == 1)
    {
        if (level == 1)
        {
            player.setMinDamage(player.getMinDamage() * 1.1f);
            player.setMaxDamage(player.getMaxDamage() * 1.1f);
        }
        else if (level == 2)
        {
            player.setMinDamage(player.getMinDamage() * 1.25f);
            player.setMaxDamage(player.getMaxDamage() * 1.25f);
        }
        else if (level == 3)
        {
            player.setMinDamage(player.getMinDamage() * 1.4f);
            player.setMaxDamage(player.getMaxDamage() * 1.4f);
        }
    }
    // Player Speed
    else if (upgrade == 2)
    {
        if (level == 1)
        {
            player.setSpeed(player.getSpeed() * 1.1f);
        }
        else if (level == 2)
        {
            player.setSpeed(player.getSpeed() * 1.2f);
        }
        else if (level == 3)
        {
            player.setSpeed(player.getSpeed() * 1.3f);
        }
    }
    // Attack Speed
    else if (upgrade == 3)
    {
        if (level == 1)
        {
            player.setAttackSpeed(sf::seconds(player.getAttackCooldownTime().asSeconds() * 0.9f));
        }
        else if (level == 2)
        {
            player.setAttackSpeed(sf::seconds(player.getAttackCooldownTime().asSeconds() * 0.8f));
        }
        else if (level == 3)
        {
            player.setAttackSpeed(sf::seconds(player.getAttackCooldownTime().asSeconds() * 0.7f));
        }
    }
    // Shot Speed
    else if (upgrade == 4)
    {
        if (level == 1)
        {
            projectileSpeed *= 1.1;
        }
        else if (level == 2)
        {
            projectileSpeed *= 1.2;
        }
        else if (level == 3)
        {
            projectileSpeed *= 1.4;
        }
    }
    // Health
    else if (upgrade == 5)
    {
        if (level == 1)
        {
            player.setMaxHealth(player.getMaxHealth() * 1.1f);
            player.heal(player.getMaxHealth());
        }
        else if (level == 2)
        {
            player.setMaxHealth(player.getMaxHealth() * 1.25f);
            player.heal(player.getMaxHealth());
        }
        else if (level == 3)
        {
            player.setMaxHealth(player.getMaxHealth() * 1.4f);
            player.heal(player.getMaxHealth());
        }
    }
    // All Stats
    else if (upgrade == 6)
    {
        if (level == 1)
        {
            player.setMinDamage(player.getMinDamage() * 1.05f);
            player.setMaxDamage(player.getMaxDamage() * 1.05f);
            player.setSpeed(player.getSpeed() * 1.03f);
            player.setAttackSpeed(sf::seconds(player.getAttackCooldownTime().asSeconds() * 0.95f));
            projectileSpeed *= 1.05;
            player.setMaxHealth(player.getMaxHealth() * 1.05f);
        }
        else if (level == 2)
        {
            player.setMinDamage(player.getMinDamage() * 1.1f);
            player.setMaxDamage(player.getMaxDamage() * 1.1f);
            player.setSpeed(player.getSpeed() * 1.05f);
            player.setAttackSpeed(sf::seconds(player.getAttackCooldownTime().asSeconds() * 0.9f));
            projectileSpeed *= 1.1;
            player.setMaxHealth(player.getMaxHealth() * 1.1f);
        }
        else if (level == 3)
        {
            player.setMinDamage(player.getMinDamage() * 1.3f);
            player.setMaxDamage(player.getMaxDamage() * 1.3f);
            player.setSpeed(player.getSpeed() * 1.2f);
            player.setAttackSpeed(sf::seconds(player.getAttackCooldownTime().asSeconds() * 0.7f));
            projectileSpeed *= 1.3;
            player.setMaxHealth(player.getMaxHealth() * 1.3f);
            player.heal(player.getMaxHealth());
        }
    }

    gameState = GameState::InGame;
    resetUpgradeScreen();
}

void Game::resetUpgradeScreen()
{
    selectedUpgrades.clear();
    selectedUpgradeLevels.clear();
    upgradeIcons.clear();
    descriptions.clear();

    upgradesSet = false;
    levelUpPoints--;
}

void Game::drawPlayerHealthBar()
{
    int length = player.getMaxHealth() / 2;
    int height = 12;
    int padding = 6;

    if (length > window.getSize().x - 20)
    {
        length = window.getSize().x - 20;
    }

    float sizePercentage = static_cast<float>(player.getHealth()) / static_cast<float>(player.getMaxHealth());

    sf::RectangleShape healthBackground(sf::Vector2f(length, height));
    sf::RectangleShape health(sf::Vector2f((length - padding) * sizePercentage, height - padding));

    healthBackground.setPosition(10, window.getSize().y - 30);
    health.setPosition(10 + padding / 2, window.getSize().y - 30 + padding / 2);

    healthBackground.setFillColor(sf::Color(20, 40, 60, 190));
    health.setFillColor(sf::Color(160, 35, 35, 180));

    window.draw(healthBackground);
    window.draw(health);
}

void Game::openClosePauseMenuEvent(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::P)
        {
            if (isPause && !gameOver)
            {
                resume();
            }
            else
            {
                pause();
            }
        }
    }
}

void Game::drawPauseMenu()
{
    if (isPause && !gameOver)
    {
        sf::RectangleShape pauseBackground(sf::Vector2f(window.getSize().x, window.getSize().x));
        pauseBackground.setFillColor(sf::Color(0, 0, 0, 150));

        // std::string text = ("CONTROLS\n Attack \n- Mouse to aim and left click to attack.\n- Alternative: Arrow keys to attack in that direction \nMovement \n- W A S D to move your character \nMenus \n- Esc to exit the program \n- P to pause the game \n- Tab to level up \n- R to restart after defeat\n");
        sf::Text pauseTitle("Pause", font, 60);
        pauseTitle.setPosition(window.getSize().x * 0.05, window.getSize().y * 0.05);

        window.draw(pauseBackground);
        window.draw(pauseTitle);
    }
}

void Game::pause()
{
    isPause = true;

    for (auto enemy : enemies)
    {
        enemy.pause();
    }

    player.pause();
}

void Game::resume()
{
    isPause = false;
    for (auto enemy : enemies)
    {
        enemy.resume();
    }

    spawnTimer.restart();

    player.resume();
}

void Game::drawDefeatMenu()
{
    if (gameOver)
    {
        sf::RectangleShape defeatBackground(sf::Vector2f(window.getSize().x, window.getSize().x));
        defeatBackground.setFillColor(sf::Color(50, 0, 0, 150));

        sf::Text defeatTitle("GAME OVER", font, 80);
        defeatTitle.setPosition(window.getSize().x * 0.4, window.getSize().y * 0.1);

        window.draw(defeatBackground);
        window.draw(defeatTitle);
    }
}

void Game::defeat()
{
    pause();
    gameOver = true;
}

void Game::reset()
{
    gameState = GameState::MainMenu;

    gameMusic.stop();

    // Projectiles
    playerProjectiles.clear();

    // Enemys
    enemies.clear();

    // Particles
    killParticles.clear();
}

void Game::resetEvent(sf::Event event)
{
    if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
    {
        reset();
    }
}

void Game::loadMusic()
{
    volume = 30;
    if (!gameMusic.openFromFile("resources/music/DodgerSoundtrack.wav"))
    {
        std::cerr << "Could not load music file!\n";
        exit(1);
    }
    gameMusic.setVolume(volume);
}

void Game::playMusic()
{
    if (gameMusic.getStatus() != sf::Sound::Playing)
    {
        gameMusic.play();
    }
}

void Game::loadFont()
{
    if (!font.loadFromFile("resources/retganon.ttf"))
    {
        return;
    }
}

void Game::shuffle(std::vector<int> &list)
{
    for (int i = 0; i < list.size(); i++)
    {
        // get random index
        int randomIndex = RandomNumber::getRandomInt(0, list.size() - 1);

        // switch current element with index
        int temp = list.at(i);
        list.at(i) = list.at(randomIndex);
        list.at(randomIndex) = temp;
    }
}

void Game::mousePositionEvent(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        std::cout << "X: " << sf::Mouse::getPosition(window).x << "Y: " << sf::Mouse::getPosition(window).y << "\n";
    }
}
