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
