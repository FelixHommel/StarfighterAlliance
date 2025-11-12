# Starfighter Alliance
![Build](https://github.com/FelixHommel/StarfighterAlliance/actions/workflows/cmake-multi-platform.yml/badge.svg)
![Coverage](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/FelixHommel/StarfighterAlliance/badges/.github/badges/coverage-badge.json)

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

## TODO List

> Items are not in any particular order

- [] Reenable tests (add\_directory)
    - Fix tests
    - figure out gmock issue
    - classes for each test type
- [] Fix All warnings
    - Turn on Warnings as errors
- [x] Websocketpp c++23 compatibility patch
    - consider switching to an alternative
    - turn everything back into c++23 mode (CMAKE\_CXX\_STANDARD, target\_compile\_features)
- [] Convert comments to Doxygen
- [] Namespaces for everything
- [] Clean up architecture
    - JSONReader class complete overhaul
    - better/different solution for ResourceManager?
- [] Resolve the color picking service issue (Server not online anymore)

## Building on Linux
### Debian based
1. Clone the repository:
    ```bash
    git clone https://github.com/FelixHommel/StarfighterAlliance.git
    ```
3. Build from source
    - Build yourself:
        ```bash
        cmake -B build -GNinja
        ```
        Use default compiler, for building without Ninja leave out -GNinja.
        Then use:
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
- [LeranOpenGL.com](https://learnopengl.com/)
- [How to use websocket++](https://medium.com/nerd-for-tech/your-first-c-websocket-client-4e7b36353d26)
