#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
class RenderObject
{
public:
	virtual void draw(sf::RenderWindow* window) = 0;
	virtual sf::FloatRect getBox() = 0;
	virtual sf::Vector2f getPosition() = 0;
	virtual sf::Color getColor() = 0;
	virtual sf::Vector2f getSize() = 0;
	virtual void setPosition(sf::Vector2f position) = 0;
	virtual void setPosition(float x, float y) = 0;
	virtual void moveDistance(sf::Vector2f distance) = 0;
	virtual void moveDistance(float x, float y) = 0;

	static sf::RenderStates getState();
	static void moveCamera(float x, float y);
	static void moveCamera(sf::Vector2f offset);

private:
	static std::mutex stateMutex;
	static sf::RenderStates renderState;
};

