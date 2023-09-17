#include "RenderRect.h"

RenderRect::RenderRect(sf::Vector2f size)
{
    shape = sf::RectangleShape(size);
}

RenderRect::RenderRect(sf::Vector2f size, sf::Vector2f position)
{
    shape = sf::RectangleShape(size);
    shape.setPosition(position);
}

RenderRect::RenderRect(sf::Vector2f size, sf::Vector2f position, sf::Color color) : RenderRect(size, position)
{
    shape.setFillColor(color);
}

void RenderRect::draw(sf::RenderWindow* window)
{
    window->draw(shape, getState());
}

sf::FloatRect RenderRect::getBox()
{
    return shape.getGlobalBounds();
}

sf::Vector2f RenderRect::getPosition()
{
    return shape.getPosition();
}

sf::Color RenderRect::getColor()
{
    return shape.getFillColor();
}

sf::Vector2f RenderRect::getSize()
{
    return shape.getSize();
}

void RenderRect::setPosition(sf::Vector2f position)
{
    shape.setPosition(position);
}

void RenderRect::setPosition(float x, float y)
{
    shape.setPosition(x, y);
}

void RenderRect::moveDistance(sf::Vector2f distance)
{
    shape.move(distance);
}

void RenderRect::moveDistance(float x, float y)
{
    shape.move(x, y);
}
