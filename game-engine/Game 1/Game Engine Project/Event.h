#pragma once
#include <string>
#include "Mobile.h"
#include <unordered_map>
class Event
{
public:
	Event(std::string type, float timestamp, float relativePriority, float age);

	std::string getType();
	float getPriority();
	float getTimestamp();
	float getAge();

	bool operator>(const Event& e);
	bool operator<(const Event& e);
	bool operator==(const Event& e);
	bool operator>=(const Event& e);
	bool operator<=(const Event& e);

private:
	std::string type;

	float priority;

	float relativePriority;

	float timestamp;

	float age;
};
