#pragma once
#include "Collision.h"
class MovingCollision :
    public Collision
{
public:
    void checkCollision(GameObject* object, Mobile* other) override;
};

