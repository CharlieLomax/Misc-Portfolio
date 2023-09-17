#include "Event.h"

Event::Event(std::string type, float timestamp, float relativePriority, float age)
{
    this->type = type;
    this->age = age;
    this->relativePriority = relativePriority;
    this->timestamp = timestamp;
    priority = timestamp + age;
}

std::string Event::getType()
{
    return type;
}

float Event::getPriority()
{
    return priority;
}

float Event::getTimestamp()
{
    return timestamp;
}

float Event::getAge()
{
    return age;
}

bool Event::operator>(const Event& e)
{
    if (priority == e.priority)
        return relativePriority < e.relativePriority;
    return priority < e.priority;
}

bool Event::operator<(const Event& e)
{
    if (priority == e.priority)
        return relativePriority > e.relativePriority;
    return priority > e.priority;
}

bool Event::operator==(const Event& e)
{
    return priority == e.priority && relativePriority == e.relativePriority;
}

bool Event::operator>=(const Event& e)
{
    return priority < e.priority || (priority == e.priority && relativePriority <= e.relativePriority);
}

bool Event::operator<=(const Event& e)
{
    return priority > e.priority || (priority == e.priority && relativePriority >= e.relativePriority);
}
