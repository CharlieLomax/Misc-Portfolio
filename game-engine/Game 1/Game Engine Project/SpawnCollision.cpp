#include "SpawnCollision.h"
#include "Mobile.h"

SpawnCollision::SpawnCollision(sf::FloatRect hitbox, sf::Vector2f point)
{
	this->hitbox = hitbox;
	this->point = point;
}

void SpawnCollision::checkCollision(GameObject* object, Mobile* other)
{
	if (hitbox.intersects(other->getBox())) {
		other->setSpawn(point);
	}
}
