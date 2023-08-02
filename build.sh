#!/bin/bash

clang++ -Wall -Werror -std=c++14 _main.cpp spaceShip.cpp arena.cpp skyBox.cpp bullet.cpp particleEffect.cpp explosion.cpp asteroid.cpp -Wno-deprecated-declarations -framework GLUT -framework OpenGL -framework Carbon -o _main\

./_main

