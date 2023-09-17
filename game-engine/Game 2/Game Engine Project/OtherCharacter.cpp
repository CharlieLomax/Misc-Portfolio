#include "OtherCharacter.h"

std::map<int, OtherCharacter*> OtherCharacter::characters = std::map<int, OtherCharacter*>();
std::mutex OtherCharacter::map_mutex;

OtherCharacter::OtherCharacter(const std::string& filename, int id) : GameObject(NULL, NULL, new RenderSprite(filename))
{
	clientId = id;
}

int OtherCharacter::getClientId()
{
	return clientId;
}

OtherCharacter* OtherCharacter::getCharacter(int id)
{
	std::lock_guard<std::mutex> lock(map_mutex);
	return characters[id];
}

std::map<int, OtherCharacter*> OtherCharacter::getCharacters()
{
	std::lock_guard<std::mutex> lock(map_mutex);
	return characters;
}

void OtherCharacter::connect(int id, const std::string& filename)
{
	std::lock_guard<std::mutex> lock(map_mutex);
	OtherCharacter* newChar = new OtherCharacter(filename, id);
	characters[id] = newChar;
}

void OtherCharacter::disconnect(int id)
{
	std::lock_guard<std::mutex> lock(map_mutex);
	delete characters[id];
	characters.erase(id);
}
