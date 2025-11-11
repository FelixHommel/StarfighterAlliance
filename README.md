# Starfighter Alliance
A 2D game about shooting Meteorites and other Spae objects, in order to achieve the highest score.

## About
In Starfighter Alliance, players will controll one of the three available spaceships, fight variuos 
forms of enemies with their spaceship and earn points while doing so. The ultimate goal of the game
is to earn as many points as possible. All scores will be saved and the Top 10 all time high scores
will be shown to the player after a game eneded.

## Features
- Works on Linux (tested on Ubuntu 24.04.01)
- Good graphics
- Different spaceships in different colors
- Various forms of enemies
- Highscore system
- Pausing while in-game

## Building on Linux
### Using the play.sh script
Just run the play.sh script and the game will launch if it is already built, or it will build itself and then launch.

### Debian based
1. Clone the repository:
    ```bash
    git clone https://gitlab.uni-ulm.de/softwaregrundprojekt/2024-2025/einzelprojekt/tutorium-julian/hommel-felix.git
    ```
2. Ensure that the dependenices are installed:
    ```bash
    sudo apt install libglfw3-dev libglm-dev libfreetype-dev libstb-dev nlohmann-json3-dev libboost-dev libwebsocketpp-dev
    ```
3. Build from source

    To build either run one of the build scripts, for either makefile or ninja (both of these use clang-19 as compiler), or

    - Build yourself:

        ```bash
        cmake -B build -GNinja
        ```
        Use default compiler, for building without Ninja leave out -GNinja.

        Then use 
        ```bash
        cmake --build build
        ```
        to build the game.

4. Run the game

    to run execute
    ```bash
    ./build/starfighterAlliance
    ```

## Acknowledgements / Credits
[LeranOpenGL.com](https://learnopengl.com/)
[How to use websocket++](https://medium.com/nerd-for-tech/your-first-c-websocket-client-4e7b36353d26)
