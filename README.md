# Project_Demo

This project is a simple game powered by SFML;

This simple project served as a test of my current skills, currently this project is a little bit of an undeveloped and poorly documented mess that I'm trying to improve in spare time;

# Builds

Here is version builded for Windows 10:
https://drive.google.com/file/d/19_S6EVS8sCoLbXo4Y7qx12gP_nuC2jIu/view?usp=sharing

# Compile

It can be compiled by typing this line if makefile won't work:
> g++ -std=c++17 -Wall src/*.cpp src/graphic/*.cpp src/input/*.cpp src/controllers/*.cpp src/abilities/*.cpp src/effects/*.cpp src/battles/*.cpp -o Demo -lsfml-graphics -lsfml-window -lsfml-system

SFML is only dependency;

# Controls

Everything in game can be controlled by mouse, press Esc to quit game;
