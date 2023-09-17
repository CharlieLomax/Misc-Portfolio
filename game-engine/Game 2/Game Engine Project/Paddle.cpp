#include "Paddle.h"
#include "PlatformCollision.h"
#include "CharacterCollision.h"

Paddle::Paddle(sf::Vector2f size, sf::Vector2f spawn) : Mobile(size, spawn, new CharacterCollision())
{
	setSpeed(0.3);
	setCollision(new PlatformCollision());
}

Paddle::Paddle(float sizeX, float sizeY, float spawnX, float spawnY) : Paddle(sf::Vector2f(sizeX, sizeY), sf::Vector2f(spawnX, spawnY))
{
}
