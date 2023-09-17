#pragma once
#include "EventHandler.h"
class ConnectHandler :
    public EventHandler
{
public:
	ConnectHandler(const std::string& filename);
	void onEvent(std::shared_ptr<Event> e) override;

private:
	std::string filename;
};

