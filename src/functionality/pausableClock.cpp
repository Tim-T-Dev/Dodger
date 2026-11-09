#include "pausableClock.hpp"

PausableClock::PausableClock()
{
    isPaused = false;
    runTime = sf::Time::Zero;
    clock.restart();
}

void PausableClock::reset()
{
    isPaused = false;
    runTime = sf::Time::Zero;
    clock.restart();
}

void PausableClock::resume()
{
    reset();
    getElapsedTime();
}

void PausableClock::pause()
{
    isPaused = true;
}

sf::Time PausableClock::getElapsedTime()
{
    if (isPaused)
    {
        return sf::Time::Zero;
    }

    runTime = clock.restart();
    if (runTime.asSeconds() > 0.05)
    {
        return sf::Time::Zero;
    }

    return runTime;
}
