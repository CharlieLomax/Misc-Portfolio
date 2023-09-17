#include "SelfDestructBlock.h"
#include "SelfDestructCollision.h"
#include "RenderRect.h"

SelfDestructBlock::SelfDestructBlock(sf::Vector2f size, sf::Vector2f position, sf::Color color) : GameObject(new SelfDestructCollision(), NULL, new RenderRect(size, position, color))
{
	
}
