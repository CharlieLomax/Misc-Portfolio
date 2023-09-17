#pragma once
#include "GameObject.h"
#include "BoundaryCollision.h"
class SideBoundary :
    public GameObject
{
public:
    SideBoundary(float posLeft, float posRight);
};

