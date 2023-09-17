#include <zmq.hpp>
#include <thread>
#include "../Game Engine Project/UntexturedMovingPlatform.h"
#include "../Game Engine Project/Timeline.h"
#include "../Game Engine Project/EventManager.h"
#include "../Game Engine Project/MoveHandler.h"
#include "ServerConnectHandler.h"
#include "../Game Engine Project/ConnectEvent.h"
#include "../Game Engine Project/DisconnectEvent.h"
#include "ServerDisconnectHandler.h"


std::string constructMessage(float clientID, float clientNum, std::map<float, sf::Vector3f>* positions, std::list<float>* disconnects) {
    std::string message;
    
    while(!disconnects->empty()) {
        float dc = disconnects->front();
        disconnects->pop_front();
        positions->erase(dc);
    }

    for (std::map<float, sf::Vector3f>::iterator it = positions->begin(); it != positions->end(); it++) {
        sf::Vector3f curr = it->second;

        if (curr.x != clientID) {
            message += "c " + std::to_string(curr.x) + " " + std::to_string(curr.y) + " " + std::to_string(curr.z) + " ";
        }

    }

    return message;
}

Timeline globalTime(NULL, 1);

void moveObjects(std::mutex* m, int* clientsConnected) {
    std::unique_lock<std::mutex> lock(*m);

    int lastTime = globalTime.getTime();
    while (*clientsConnected > 0) {
        lock.unlock();
        float delta_t = (float)(globalTime.getTime() - lastTime);
        lastTime = globalTime.getTime();

        for (GameObject* object : GameObject::getObjects()) {
            object->moveObject(delta_t);
        }

        lock.lock();
    }
}

int main() {
    //  Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    std::mutex m;

    float clientNum = 0;
    int clientsConnected = 0;

    // Create event handlers
    ServerConnectHandler serverConnectHandler(&socket);
    ServerDisconnectHandler serverDisconnectHandler(&socket);

    std::map<float, sf::Vector3f> characterPositions;

    std::list<float> disconnects;

    zmq::message_t request;

    //  Wait for next request from client
    socket.recv(request);

    // Acknowledge to the client that it has joined, and tell it its client number
    zmq::message_t joinReply(sizeof(float));
    memcpy(joinReply.data(), &clientNum, sizeof(float));
    socket.send(joinReply, zmq::send_flags::none);
    EventManager::instance()->raise(new ConnectEvent(globalTime.getTime(), 0.f, 0.f, clientNum));
    clientNum++;
    clientsConnected++;

    std::thread moveThread(moveObjects, &m, &clientsConnected);

    while (clientsConnected > 0) {

        zmq::message_t request;

        //  Wait for next request from client
        socket.recv(request);

        // If the client is joining, the message size will be zero
        if (request.size() == 0) {
            // Acknowledge to the client that it has joined, and tell it its client number
            zmq::message_t joinReply(4);
            memcpy(joinReply.data(), &clientNum, 4);
            socket.send(joinReply, zmq::send_flags::none);
            EventManager::instance()->raise(new ConnectEvent(globalTime.getTime(), 0.f, 0.f, clientNum));
            clientNum++;
            clientsConnected++;
        }

        else {
            // The first value in the message will always be the client ID
            float* ptr = (float*)request.data();
            float client = *ptr;

            if (request.size() == 3 * sizeof(float)) {
                
                if (characterPositions.count(client) == 0) {
                    sf::Vector3f data(client, *(ptr + 1), *(ptr + 2));
                    characterPositions.emplace(client, data);
                }

                else {
                    sf::Vector3f* curr = &characterPositions.at(client);
                    curr->y = *(ptr + 1);
                    curr->z = *(ptr + 2);
                }

            }

            else if (request.size() == 2 * sizeof(float) && *(ptr + 1) == -1.f) {
                clientsConnected--;
                disconnects.push_back(client);
                EventManager::instance()->raise(new DisconnectEvent(globalTime.getTime(), 0.f, 0.f, client));
            }

            std::string message = constructMessage(client, clientNum, &characterPositions, &disconnects);
                    
            //  Send reply back to client containing the constructed message
            int replySize = 1;
            if (message.size() > 0)
                replySize = message.size();
            zmq::message_t reply(replySize);
            memcpy(reply.data(), message.c_str(), replySize);
            socket.send(reply, zmq::send_flags::none);
        }


        EventManager::instance()->manage();
    }

    moveThread.join();
    return 0;
}