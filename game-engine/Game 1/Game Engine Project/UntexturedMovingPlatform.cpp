#include "UntexturedMovingPlatform.h"

// Velocity is calculated using the unit vector between the first two points on the path multiplied by the speed
UntexturedMovingPlatform::UntexturedMovingPlatform(sf::Vector2f size, sf::Vector2f position, float moveSpeed, std::vector<sf::Vector2f> turnsVector) : GameObject(new MovingCollision(), new PlatformMove(moveSpeed, turnsVector), new RenderRect(size, position), hat(turnsVector.front() - position)* moveSpeed) {
    
}

UntexturedMovingPlatform::UntexturedMovingPlatform(sf::Vector2f size, sf::Vector2f position, float moveSpeed, std::vector<sf::Vector2f> turnsVector, sf::Color color) : GameObject(new MovingCollision(), new PlatformMove(moveSpeed, turnsVector), new RenderRect(size, position, color), hat(turnsVector.front() - position)* moveSpeed)
{

}

sf::Vector2f UntexturedMovingPlatform::hat(sf::Vector2f vector) {
	return vector / sqrt((vector.x * vector.x) + (vector.y * vector.y));
}
