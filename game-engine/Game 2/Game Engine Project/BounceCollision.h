#pragma once
#include "MobileCollision.h"
#include "Mobile.h"
class BounceCollision : 
	public MobileCollision
{
public:
	BounceCollision(Mobile* obj);

    void clearCollision() override;

    bool getLeftCollision() override;
    bool getRightCollision() override;
    bool getUpCollision() override;
    bool getDownCollision() override;

    void collideLeft() override;
    void collideRight() override;
    void collideUp() override;
    void collideDown() override;

private:
    Mobile* obj;
    bool bounced;
};

