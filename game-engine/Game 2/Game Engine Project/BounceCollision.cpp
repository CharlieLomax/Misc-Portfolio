#include "BounceCollision.h"

BounceCollision::BounceCollision(Mobile* obj)
{
	this->obj = obj;
	bounced = false;
}

void BounceCollision::clearCollision()
{
	bounced = false;
}

bool BounceCollision::getLeftCollision()
{
	return false;
}

bool BounceCollision::getRightCollision()
{
	return false;
}

bool BounceCollision::getUpCollision()
{
	return false;
}

bool BounceCollision::getDownCollision()
{
	return false;
}

void BounceCollision::collideLeft()
{
	if (!bounced) {
		sf::Vector2f v = obj->getVelocity();
		obj->setVelocity(sf::Vector2f(-v.x, v.y));
		bounced = true;
	}
}

void BounceCollision::collideRight()
{
	if (!bounced) {
		sf::Vector2f v = obj->getVelocity();
		obj->setVelocity(sf::Vector2f(-v.x, v.y));
		bounced = true;
	}
}

void BounceCollision::collideUp()
{
	if (!bounced) {
		sf::Vector2f v = obj->getVelocity();
		obj->setVelocity(sf::Vector2f(v.x, -v.y));
		bounced = true;
	}
}

void BounceCollision::collideDown()
{
	if (!bounced) {
		sf::Vector2f v = obj->getVelocity();
		obj->setVelocity(sf::Vector2f(v.x, -v.y));
		bounced = true;
	}
}
