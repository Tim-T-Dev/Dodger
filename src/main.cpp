#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

int main()
{
    Game game(1280, 720);
    game.start();
    return 0;
}
