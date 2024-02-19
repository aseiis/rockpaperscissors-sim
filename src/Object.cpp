#include "../include/Object.h"

Object::Object(TextureManager* tm, int ID) : ID(ID), textureManager(tm)
{
    sprite = sf::Sprite();
}

void Object::setPosition(const sf::Vector2f& newPos)
{
    position = newPos;
    sprite.setPosition(newPos);
}

sf::Vector2f Object::getPosition()
{
    return position;
}

void Object::setTexture(sf::Texture& texture)
{
    sprite.setTexture(texture);
    sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
    sprite.setScale(baseSpriteScale, baseSpriteScale);
    sprite.setPosition(position.x, position.y);
}

void Object::draw(sf::RenderWindow& window, bool debug_mode)
{
    window.draw(sprite);

    if(debug_mode){
        sf::CircleShape debugPoint;
        debugPoint.setRadius(2.f);
        debugPoint.setFillColor(sf::Color(255, 0, 0));
        debugPoint.setPosition(position);
        window.draw(debugPoint);
    }
}

void Object::move()
{
    setPosition(position + velocity);
}

void Object::mutate(ObjectType newType)
{
    //std::cout << "Mutating into " << static_cast<int>(newType) << "\n";
    type = newType;
    if(newType == ObjectType::ROCK){
        //std::cout << "Setting rock texture.." << std::endl;
        setTexture(textureManager->rockTexture);
    } else if(newType == ObjectType::PAPER){
        //std::cout << "Setting paper texture.." << std::endl;
        setTexture(textureManager->paperTexture);
    } else if(newType == ObjectType::SCISSORS){
        //std::cout << "Setting scissors texture.." << std::endl;
        setTexture(textureManager->scissorsTexture);
    } else {
        std::cout << "Error while trying to modify texture when mutating";
    }
}

float Object::getRealRadius()
{
    return 108 * baseSpriteScale;
}