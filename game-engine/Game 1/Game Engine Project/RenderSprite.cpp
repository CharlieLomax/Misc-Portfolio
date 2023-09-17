#include "RenderSprite.h"

RenderSprite::RenderSprite(const std::string& filename)
{
    texture = sf::Texture();
    texture.loadFromFile(filename);
    sprite = sf::Sprite(texture);
}

RenderSprite::RenderSprite(const std::string& filename, sf::Color color) : RenderSprite(filename)
{
    sprite.setColor(color);
}

void RenderSprite::draw(sf::RenderWindow* window)
{
    window->draw(sprite, getState());
}

sf::FloatRect RenderSprite::getBox()
{
    return sprite.getGlobalBounds();
}

sf::Vector2f RenderSprite::getPosition()
{
    return sprite.getPosition();
}

sf::Color RenderSprite::getColor()
{
    return sprite.getColor();
}

sf::Vector2f RenderSprite::getSize()
{
    return sf::Vector2f(sprite.getLocalBounds().width, sprite.getLocalBounds().height);
}

void RenderSprite::setPosition(sf::Vector2f position)
{
    sprite.setPosition(position);
}

void RenderSprite::setPosition(float x, float y)
{
    sprite.setPosition(x, y);
}

void RenderSprite::moveDistance(sf::Vector2f distance)
{
    sprite.move(distance);
}

void RenderSprite::moveDistance(float x, float y)
{
    sprite.move(x, y);
}
