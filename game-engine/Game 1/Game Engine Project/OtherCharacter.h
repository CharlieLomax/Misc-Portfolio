#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "RenderSprite.h"
#pragma once
class OtherCharacter :
    public GameObject
{
public:
    OtherCharacter(const std::string& filename, int id);
    int getClientId();
    static std::map<int, OtherCharacter*> getCharacters();
    static OtherCharacter* getCharacter(int id);
    static void connect(int id, const std::string& filename);
    static void disconnect(int id);

private:
    int clientId;
    static std::map<int, OtherCharacter*> characters;
    static std::mutex map_mutex;
};

