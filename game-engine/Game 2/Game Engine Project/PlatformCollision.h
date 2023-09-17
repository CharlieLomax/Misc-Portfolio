#pragma once
#include "Collision.h"
class PlatformCollision :
    public Collision
{
public:
    void checkCollision(GameObject* object, Mobile* other) override;
};

