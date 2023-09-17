#include <zmq.hpp>
#include <string>
#include <iostream>

int main()
{
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);

    socket.connect("tcp://localhost:5555");

    zmq::message_t joinMessage(1);
    socket.send(joinMessage, zmq::send_flags::none);
    zmq::message_t joinReply;
    socket.recv(joinReply);

    while (true) {
        zmq::message_t request(0);
        socket.send(request, zmq::send_flags::none);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv(reply);

        std::string replyContent((char*)reply.data(), reply.size());
        std::cout << replyContent << std::endl;
    }
    return 0;
}

// Based on ZMQ example code from https://zguide.zeromq.org/docs/chapter1/