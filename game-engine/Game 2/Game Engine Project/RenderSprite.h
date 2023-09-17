#pragma once
#include "RenderObject.h"
class RenderSprite :
    public RenderObject
{
public:
	RenderSprite(const std::string& filename);
	RenderSprite(const std::string& filename, sf::Color color);
	
	void draw(sf::RenderWindow* window) override;
	sf::FloatRect getBox() override;
	sf::Vector2f getPosition() override;
	sf::Color getColor() override;
	sf::Vector2f getSize() override;
	void setPosition(sf::Vector2f position) override;
	void setPosition(float x, float y) override;
	void moveDistance(sf::Vector2f distance) override;
	void moveDistance(float x, float y) override;

private:
	sf::Texture texture;
	sf::Sprite sprite;
};

