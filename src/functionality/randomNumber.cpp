#include "randomNumber.hpp"

int RandomNumber::getRandomInt(int min, int max)
{
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}