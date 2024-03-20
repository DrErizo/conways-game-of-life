#!/bin/bash

cmake . -Bbuild -GNinja -DCMAKE_EXPORT_COMPILE_COMMAND=TRUE
cmake --build build
cd build 
./conways-game-of-life
cd ..
