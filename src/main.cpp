#include <iostream>
#include <string>

#include "game.h"
#include "renderer.h"


using namespace asteroids;

int main(int argc, char* argv[])
{
    
 	Game game(60);
    game.init();
    game.run();

    return 0;
}
