#pragma once
#include "Collision.h"
class DeathCollision :
    public Collision
{
    void checkCollision(GameObject* object, Mobile* other) override;
};

