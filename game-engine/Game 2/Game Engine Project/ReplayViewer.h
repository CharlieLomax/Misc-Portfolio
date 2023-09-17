#pragma once
#include "EventManager.h"
class ReplayViewer :
    public EventHandler
{
public:
    ReplayViewer(sf::RenderWindow* window);
    void startRecording();
    void stopRecording();
    void playRecording(float speed);
    void onEvent(std::shared_ptr<Event> e) override;
    bool isRecording();
private:
    std::map<int, sf::Vector2f> startPositions;
    std::map<int, sf::Vector2f> endPositions;

    bool recording;
    std::list<std::shared_ptr<Event>> events;

    sf::RenderWindow* window;
};

extern Timeline gobalTime;

