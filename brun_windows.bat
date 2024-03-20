@echo off

cmake . -B build

cd build 
start devenv /Run conways-game-of-life.sln
cd ..
