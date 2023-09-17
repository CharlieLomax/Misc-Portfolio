#include <zmq.hpp>
#include <string>
#include <iostream>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n)	Sleep(n)
#endif

int main() {
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    int iteration = 0;
    std::vector<int> clientStart;

    while (true) {
        std::string message;
        for (int i = 0; i < (int)clientStart.size(); i++) {
            message += "Client " + std::to_string(i) + ": Iteration " + std::to_string(iteration - clientStart.at(i)) + "\n";
        }

        if (clientStart.size() == 0) {
            zmq::message_t request;

            //  Wait for next request from client
            socket.recv(request);

            // If the client is joining, the message size will be one
            if (request.size() == 1) {
                clientStart.emplace_back(iteration);

                // Acknowledge to the client that it has joined
                zmq::message_t joinReply;
                socket.send(joinReply, zmq::send_flags::none);
            }
        }

        else {
            for (int i = 0; i < (int)clientStart.size(); i++) {
                zmq::message_t request;

                //  Wait for next request from client
                socket.recv(request);

                // If the client is joining, the message size will be one
                if (request.size() == 1) {
                    clientStart.emplace_back(iteration);

                    // Acknowledge to the client that it has joined
                    zmq::message_t joinReply;
                    socket.send(joinReply, zmq::send_flags::none);
                }

                else {
                    //  Send reply back to client containing the constructed message
                    zmq::message_t reply(message.size());
                    memcpy(reply.data(), message.c_str(), message.size());
                    socket.send(reply, zmq::send_flags::none);
                }

            }

            iteration++;
        }
    }
    return 0;
}

// Based on ZMQ example code from https://zguide.zeromq.org/docs/chapter1/