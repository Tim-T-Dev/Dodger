#pragma once
#include <SFML/Graphics.hpp>

class PausableClock
{

public:
    PausableClock();

    void reset();
    void resume();
    void pause();
    sf::Time getElapsedTime();

private:
    sf::Clock clock;
    sf::Time runTime;
    bool isPaused;
};