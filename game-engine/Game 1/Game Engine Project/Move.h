#pragma once
#include <SFML/Graphics.hpp>

class GameObject;

class Move
{
public:
	virtual void move(GameObject* object, float delta_t) = 0;
};
