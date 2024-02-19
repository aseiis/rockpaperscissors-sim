#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/TextureManager.h"

enum ObjectType {
    ROCK     = 0,
    PAPER    = 1,
    SCISSORS = 2
};

class Object {
public:
    Object(TextureManager* textureManager, int ID);
    void setPosition(const sf::Vector2f& newPos);
    sf::Vector2f getPosition();
    void setTexture(sf::Texture& texture);
    void draw(sf::RenderWindow& window, bool debug_mode);
    void move();
    void mutate(ObjectType newType);
    static float getRealRadius();

    const int ID;
    ObjectType type;
    sf::Vector2f velocity;
    static constexpr float baseSpriteScale = 0.25f;
    sf::Sprite sprite;
    TextureManager* textureManager;

private:
    sf::Vector2f position;

};