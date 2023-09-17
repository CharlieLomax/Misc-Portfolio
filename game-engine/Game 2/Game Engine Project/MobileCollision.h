#pragma once
class MobileCollision
{
public:
	virtual void clearCollision() = 0;

    virtual bool getLeftCollision() = 0;
    virtual bool getRightCollision() = 0;
    virtual bool getUpCollision() = 0;
    virtual bool getDownCollision() = 0;

    virtual void collideLeft() = 0;
    virtual void collideRight() = 0;
    virtual void collideUp() = 0;
    virtual void collideDown() = 0;
};

