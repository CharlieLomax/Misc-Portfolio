#pragma once
#include <SFML/Graphics.hpp>
#include <mutex>
#include "GameObject.h"
#include "RenderSprite.h"
#include "RenderRect.h"
#include "Timeline.h"
#include "MobileCollision.h"

extern Timeline globalTime;

class Mobile : public GameObject
{
public:
    Mobile();
    
    Mobile(const std::string& filename, sf::Vector2f spawn, MobileCollision* mobileCollision);

    Mobile(sf::Vector2f size, sf::Vector2f spawn, MobileCollision* mobileCollision);

    Mobile(const Mobile&);
    Mobile(const Mobile&&);

    void operator=(const Mobile&);

    /* Resets all collision values to false */
    void clearCollision();

    bool getLeftCollision();
    bool getRightCollision();
    bool getUpCollision();
    bool getDownCollision();

    void collideLeft();
    void collideRight();
    void collideUp();
    void collideDown();

    void setMovingRight(bool movingRight);
    void setMovingLeft(bool movingLeft);

    void setMoved(bool moved);

    void inputVelocity();

    /* Move the character down due to gravity */
    void fall();

    /* The entity's behavior when it encounters a death zone */
    virtual void die();
    void respawn();

    sf::Vector2f getSpawn();
    void setSpawn(sf::Vector2f);

    void startJump();
    void jumpVelocity();

    float getSpeed();
    void setSpeed(float speed);

    void setMobileCollision(MobileCollision* collision);

private:
    bool movingRight;
    bool movingLeft;

    bool moved;

    sf::Vector2f spawn;

    int jumpStart;
    bool jump;

    float speed;

    MobileCollision* mobileCollision;
};

