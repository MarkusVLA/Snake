#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#define WIDTH 800
#define HEIGHT 800
#define CANVAS_WIDTH 32
#define CANVAS_HEIGHT 32
#define SIZE 1
#define FPS 20

struct State {
    sf::RenderWindow window;
    sf::RenderTexture canvas;
    State(){
        window.create(sf::VideoMode({WIDTH, HEIGHT}, 8), "Sanke");
        window.setFramerateLimit(FPS);
        if (!canvas.create({CANVAS_WIDTH, CANVAS_HEIGHT})){
            std::cerr << "Unable to create render texture" << std::endl;
        }
    }
};


extern struct State globalState;

