#include "RenderObject.h"

sf::RenderStates RenderObject::renderState = sf::RenderStates(sf::Transform());
std::mutex RenderObject::stateMutex;

sf::RenderStates RenderObject::getState()
{
	std::unique_lock<std::mutex> lock(stateMutex);
	return renderState;
}

void RenderObject::moveCamera(float x, float y)
{
	std::unique_lock<std::mutex> lock(stateMutex);
	renderState.transform.translate(-x, -y);
}

void RenderObject::moveCamera(sf::Vector2f offset)
{
	std::unique_lock<std::mutex> lock(stateMutex);
	renderState.transform.translate(-offset);
}
