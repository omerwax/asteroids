#include <iostream>
#include <string>

#include "game.h"
#include "renderer.h"


using namespace asteroids;

int main(int argc, char* argv[])
{
    
 	Game game(60);
    if (!game.init()){
        std::cout << "Game init failed\nExiting" << std::endl;
        return -1;
    }

    std::cout << "Game is initializied" << std::endl;

    game.run();

    std::cout << "Exiting Game" << std::endl;

    return 0;
}
