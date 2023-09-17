#include "SideBoundary.h"

SideBoundary::SideBoundary(float posLeft, float posRight) : GameObject(new BoundaryCollision(posLeft, posRight), NULL, NULL)
{
	
}
