#include <SFML/Graphics.hpp>
#include <mutex>
#include "Timeline.h"
#include "Mobile.h"
#pragma once

extern Timeline globalTime;

class Character :
    public Mobile
{
    public:
        Character(const std::string& filename, sf::Vector2f spawn);

        Character(const std::string& filename, float spawnX, float spawnY);

        void die() override;

};

