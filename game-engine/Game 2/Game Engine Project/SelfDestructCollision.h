#pragma once
#include "Collision.h"
class SelfDestructCollision :
    public Collision
{
    void checkCollision(GameObject* object, Mobile* other) override;
};

