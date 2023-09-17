#include "ServerDisconnectHandler.h"
#include "../Game Engine Project/DisconnectEvent.h"

ServerDisconnectHandler::ServerDisconnectHandler(zmq::socket_t* socket) : EventHandler(std::list<std::string>({ "Disconnect" }))
{
    this->socket = socket;
}

void ServerDisconnectHandler::onEvent(std::shared_ptr<Event> e)
{
    if (e->getType() == "Disconnect") {
        DisconnectEvent event = *std::static_pointer_cast<DisconnectEvent>(e);

        std::string message;

        message += "d " + std::to_string(event.getId());

        //  Send reply back to client containing the constructed message
        zmq::message_t reply(message.size());
        memcpy(reply.data(), message.c_str(), message.size());

        zmq::message_t request;
        socket->recv(request);

        socket->send(reply, zmq::send_flags::none);
    }
}