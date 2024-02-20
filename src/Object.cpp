#include "../include/Object.h"

Object::Object(TextureManager* tm, int& ID) : m_ID(ID), textureManager(tm)
{
    m_sprite = sf::Sprite();
    ID++;
}

void Object::setPosition(const sf::Vector2f& newPos)
{
    m_position = newPos;
    m_sprite.setPosition(newPos);
}

sf::Vector2f Object::getPosition()
{
    return m_position;
}


void Object::setObjectTexture(sf::Texture& texture)
{
    m_sprite.setTexture(texture);
    m_sprite.setOrigin(m_sprite.getLocalBounds().width/2, m_sprite.getLocalBounds().height/2);
    m_sprite.setScale(baseSpriteScale, baseSpriteScale);
    m_sprite.setPosition(m_position.x, m_position.y);
}

void Object::updateTexture()
{
    switch (this->m_type) {
        case ObjectType::ROCK:
            this->setObjectTexture(textureManager->rockTexture);
            break;
        case ObjectType::PAPER:
            this->setObjectTexture(textureManager->paperTexture);
            break;
        case ObjectType::SCISSORS:
            this->setObjectTexture(textureManager->scissorsTexture);
            break;
        default:
            std::cout << "ERROR: Couldn't load texture based on type: type detection failed";
            break;
    }
}

void Object::draw(sf::RenderWindow& window, bool debug_mode)
{
    window.draw(m_sprite);

    if(debug_mode){
        sf::CircleShape debugPoint;
        debugPoint.setRadius(3.f);
        debugPoint.setFillColor(sf::Color(255, 0, 0));
        debugPoint.setOutlineColor(sf::Color(255, 100, 100));
        debugPoint.setOutlineThickness(1.f);
        debugPoint.setPosition(m_position);
        window.draw(debugPoint);
    }
}

void Object::move()
{
    setPosition(m_position + m_velocity);
}

void Object::mutate(ObjectType newType)
{
    //std::cout << "Trying to mutate into " << static_cast<int>(newType) << "\n";

    if(newType >= ObjectType::_LENGTH){
        std::cout << "Error when mutating: unknown object type" << std::endl;
        return;
    }

    m_type = newType;
    this->updateTexture();
}

void Object::mutateToNext()
{
    this->mutate(static_cast<ObjectType>((this->m_type + 1) % _LENGTH));
}

float Object::getRealRadius()
{
    return SPRITE_SIZE * baseSpriteScale;
}