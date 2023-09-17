#pragma once
#include <zmq.hpp>
#include "../Game Engine Project/EventHandler.h"
class ServerConnectHandler :
	public EventHandler
{
public:
	ServerConnectHandler(zmq::socket_t* socket);
	void onEvent(std::shared_ptr<Event> e) override;

private:
	zmq::socket_t* socket;
};

