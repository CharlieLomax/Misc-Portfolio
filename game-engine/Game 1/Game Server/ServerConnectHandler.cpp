#include "ServerConnectHandler.h"
#include "../Game Engine Project/ConnectEvent.h"

ServerConnectHandler::ServerConnectHandler(zmq::socket_t* socket) : EventHandler(std::list<std::string>({ "Connect" }))
{
	this->socket = socket;
}

void ServerConnectHandler::onEvent(std::shared_ptr<Event> e)
{
    if (e->getType() == "Connect") {
        ConnectEvent event = *std::static_pointer_cast<ConnectEvent>(e);

        std::string message;

        message += "n " + std::to_string(event.getId());

        //  Send reply back to client containing the constructed message
        zmq::message_t reply(message.size());
        memcpy(reply.data(), message.c_str(), message.size());

        zmq::message_t request;
        socket->recv(request);

        socket->send(reply, zmq::send_flags::none);
    }
}
