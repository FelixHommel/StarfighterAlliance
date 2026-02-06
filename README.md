# Starfighter Alliance

![Build](https://github.com/FelixHommel/StarfighterAlliance/actions/workflows/build.yml/badge.svg)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/efae3d2fc84948c68ec4fb1330994353)](https://app.codacy.com/gh/FelixHommel/StarfighterAlliance/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Codacy Badge](https://app.codacy.com/project/badge/Coverage/efae3d2fc84948c68ec4fb1330994353)](https://app.codacy.com/gh/FelixHommel/StarfighterAlliance/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_coverage)

A 2D game about shooting Meteorites and other Space objects, in order to achieve the highest score.
This game was developed during winter semester 2024/2025 @ Ulm University.

## About

In Starfighter Alliance, players will control one of the three available spaceships, fight various 
forms of enemies with their spaceship and earn points while doing so. The ultimate goal of the game
is to earn as many points as possible. All scores will be saved and the Top 10 all-time high scores
will be shown to the player after a game ended.

## Features

- Works on Linux (tested on Ubuntu 24.04.01)
- Good graphics
- Different spaceships in different colors
- Various forms of enemies
- Highscore system
- Pausing while in-game

## TODO List

> Items are not in any particular order

- [x] Re-enable tests
  - Fix tests
  - Figure out gmock issue
  - Classes for each test type
- [x] Fix All warnings
  - Turn on Warnings as errors
- [x] Convert comments to Doxygen
- [x] Namespaces for everything
- [x] Clean up architecture
  - JSONReader class complete overhaul
  - Better/Different solution for ResourceManager?
    - Multithreaded resource loading
- [ ] Resolve the color picking service issue (Server is offline)
  - Provide simple custom server to self host
  - Make color random
  - Make color pickable

## Notes on Tests in CI

The Windows CI environment is not able to run tets that require OpenGL. Because of that, tests that require OpenGL are
labeled as such and can be filtered out by ctest.

## Building

1. Configure CMake

    ```bash
    cmake --preset release
    ```

2. Build the Application

    ```bash
    cmake --build --preset release
    ```

3. Run the Application

    ```bash
    ./build/release/src/app/StarfighterAlliance
    ```

## Acknowledgments / Credits

- [LearnOpenGL.com](https://learnopengl.com/)
