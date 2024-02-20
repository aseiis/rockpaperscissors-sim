#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/TextureManager.h"

enum ObjectType {
    ROCK     = 0,
    PAPER    = 1,
    SCISSORS = 2,
    //trick to get the length
    _LENGTH
};

const int SPRITE_SIZE = 108;

class Object {
public:
    Object(TextureManager* textureManager, int& ID);
    void setPosition(const sf::Vector2f& newPos);
    sf::Vector2f getPosition();
    void setObjectTexture(sf::Texture& texture);
    void updateTexture();
    void draw(sf::RenderWindow& window, bool debug_mode);
    void move();
    void mutate(ObjectType newType);
    void mutateToNext();
    static float getRealRadius();

    const int m_ID;
    ObjectType m_type;
    sf::Vector2f m_velocity;
    static constexpr float baseSpriteScale = 0.25f;
    sf::Sprite m_sprite;
    TextureManager* textureManager;

private:
    sf::Vector2f m_position;

};