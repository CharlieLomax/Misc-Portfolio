#include "HiddenObject.h"

HiddenObject::HiddenObject(sf::Vector2f size, sf::Vector2f position)
{
    this->size = size;
    this->position = position;
}

void HiddenObject::draw(sf::RenderWindow* window)
{
}

sf::FloatRect HiddenObject::getBox()
{
    return sf::FloatRect(position, size);
}

sf::Vector2f HiddenObject::getPosition()
{
    return position;
}

sf::Color HiddenObject::getColor()
{
    return sf::Color();
}

sf::Vector2f HiddenObject::getSize()
{
    return size;
}

void HiddenObject::setPosition(sf::Vector2f position)
{
    this->position = position;
}

void HiddenObject::setPosition(float x, float y)
{
    this->position = sf::Vector2f(x, y);
}

void HiddenObject::moveDistance(sf::Vector2f distance)
{
    position += distance;
}

void HiddenObject::moveDistance(float x, float y)
{
    position.x += x;
    position.y += y;
}
