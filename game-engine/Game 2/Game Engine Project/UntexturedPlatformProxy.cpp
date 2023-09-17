#include "UntexturedPlatformProxy.h"
UntexturedPlatformProxy::UntexturedPlatformProxy(sf::Vector2f size, sf::Vector2f position, sf::Vector2f velocity, sf::Color color, int serverID) : GameObject(new MovingCollision(), NULL, new RenderRect(size, position, color), velocity)
{
    this->serverID = serverID;
}