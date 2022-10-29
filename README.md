# C++ Asteroids Game

This repository contains a simplified Asteroids game.
it is implemented in C++ and the SDL2 library.
This is the final project for the Udacity C++ nanodegree program

- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://www.libsdl.org/)

## External Dependecies

The project depends on SDL2 library and the SDL2_ttf module.
On a Linux machine with a debian package manager,
the modules can be installed with the following command:

```sh
sudo apt-get install libsdl2-dev && sudo apt-get install libsdl2-ttf-dev
```
Other machines types installation instructions:
[here](https://wiki.libsdl.org/Installation)



## Cloning, Building and Running

- Clone this project
```sh
git clone git@github.com:omerwax/asteroids.git
```
- Build
```sh
cd asteroids
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ../
make
```
- Run
```sh
./asteroids
```
- Enjoy!!!

## Directory and Files structure
- src/ - a folder that contains all the required source code files
- include - afolder that contains all the required header files
- fonts/ - folder that contains TrueType font files required for text rendering
- cmake/ - a folder that contains cmake modules for finding SDL2 and SDL2_ttf libs automatically, 
           taken from [here](https://github.com/toivjon/sdl2-space-invaders)
- CmakeLists.txt - Cmake file for project build configuration
- README.md - this readme file

## Classes structure
- Game  - The Main class, responsible for managing the game, and running the main game loop;
- Renderer - A wraper class for the using SDL2 in a simpler way, renderer takes a vector of Drawable Objects and render them to the screen
- DrawableEntity - A class of renderables objects, with shape and text. The shpes are defined by a collection of rectangles.
- MovingEntity - Inherites from the DrawableEntity class, adds movment capabilities
- Spacship - Inherits from the MovingEntity class, add movement control and shooting 
- Missile - Inherits from the MovingEntity class
- Asteroid - Inherited from the MovingEntity class

## Game Behavior
- The game starts at the start screen, prompting the player to enter his name and start playing the game
- Once the game starts, there is a single spaceship and a single asteroid on the screen
- Asteroids are added to the screen every time interval
- The purpose of the game is to hit the asteroid with missiles, launched by hitting space
- If an asteroid hits the spaceship, the game is over
- Every asteroid hit gives a 100 points
- Every 1000 points, the level is increased and the game gets harder, i.e. asteroid move faster and are generated more frequently
- If the player is too tired or excited, the game can be paused by pressing ESC and then another ESC for resuming the game
- When the game is over, it can be restarted easily by pressing enter and starting all over again.
- High score is managed in a data file, data os obfuscatd in base64,
        base64 code is taken from [here](https://gist.github.com/tomykaira/f0fd86b6c73063283afe550bc5d77594)

# Project Rubric
## Readme
- A README with instructions is included with the project - Done
- The README indicates which project is chosen - Done
- The README includes information about each rubric point addressed.
## Compiling and Testing
- The submission must compile and run - The project uses cmake / make build system, it builds without any errors (tested on Ubuntu 18.04 machine)
## Loops, Functions, I/O
- The project demonstrates an understanding of C++ functions and control structure - the code is organized in fucntions, and uses while, for, switch and if control loops on various cases
## Object Oriented Programming
- The project uses Object Oriented Programming techniques - The code is organized in classes with inheritance usage
- Classes use appropriate access specifiers for class members - member functions and variables are using different access control, expilicitly
- Class constructors utilize member initialization lists - [Here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/game.h#L28)
- Classes encapsulate behavior - [Here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/spaceship.h#L17)
## Memory Management
- The project makes use of references in function declarations. [here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/renderer.cpp#L62) and [here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/game.cpp#L331)
- The project uses smart pointers instead of raw pointers - [here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/game.cpp#L20)
 - The project uses move semantics to move data, instead of copying it, where possible. - [here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/drawable_entity.h#L47) and [here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/game.cpp#L28)
 - The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. [here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/game.cpp#L20)
 - The project uses destructors appropriately - [here](https://github.com/omerwax/asteroids/blob/8ba5cb90a970382fa36c02913d1288abfb8b14be/src/renderer.cpp#L50)












