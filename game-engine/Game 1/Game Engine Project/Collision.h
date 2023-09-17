#pragma once
#include <SFML/Graphics.hpp>

class GameObject;
class Mobile;

class Collision
{
public:
	virtual void checkCollision(GameObject* object, Mobile* other) = 0;
};

