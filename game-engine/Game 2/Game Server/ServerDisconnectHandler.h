#pragma once
#include "..\Game Engine Project\EventHandler.h"
#include <zmq.hpp>
class ServerDisconnectHandler :
    public EventHandler
{
public:
	ServerDisconnectHandler(zmq::socket_t* socket);
	void onEvent(std::shared_ptr<Event> e) override;

private:
	zmq::socket_t* socket;
};

