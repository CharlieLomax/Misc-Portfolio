#include "Ball.h"
#include "BasicMove.h"
#include "BounceCollision.h"

Ball::Ball()
{
}

Ball::Ball(sf::Vector2f size, sf::Vector2f spawn, sf::Vector2f velocity) : Mobile(size, spawn, new BounceCollision(this))
{
	setMove(new BasicMove());
	setSpeed(0);
	setVelocity(velocity);
}

Ball::Ball(float sizeX, float sizeY, float spawnX, float spawnY, float velocityX, float velocityY) : Ball(sf::Vector2f(sizeX, sizeY), sf::Vector2f(spawnX, spawnY), sf::Vector2f(velocityX, velocityY))
{
}

void Ball::bounceSide()
{
	sf::Vector2f v = getVelocity();
	setVelocity(sf::Vector2f(-v.x, v.y));
}

void Ball::bounceVertical()
{
	sf::Vector2f v = getVelocity();
	setVelocity(sf::Vector2f(v.x, -v.y));
}
