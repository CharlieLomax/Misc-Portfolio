#include <SFML/Graphics.hpp>
#include <zmq.hpp>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "UntexturedPlatform.h"
#include "UntexturedMovingPlatform.h"
#include "Character.h"
#include "Timeline.h"
#include "UntexturedPlatformProxy.h"
#include "OtherCharacter.h"
#include "Mobile.h"
#include <set>
#include "SideBoundary.h"
#include "SpawnPoint.h"
#include "DeathZone.h"
#include "MoveHandler.h"
#include "EventManager.h"
#include "DeathHandler.h"
#include "ScrollHandler.h"
#include "CollisionHandler.h"
#include "SpawnHandler.h"
#include "AgentMoveHandler.h"
#include "InputHandler.h"
#include "InputEvent.h"
#include "RepositionHandler.h"
#include "RepositionEvent.h"
#include "ConnectEvent.h"
#include "ConnectHandler.h"
#include "DisconnectHandler.h"
#include "DisconnectEvent.h"
#include "ReplayViewer.h"
#include "RecordStartHandler.h"
#include "RecordStopHandler.h"
#include "DrawHandler.h"
#include "DrawEvent.h"
#include "ScriptManager.h"
#include <dukglue/dukglue.h>

sf::RenderWindow* getWindow() {
    static std::mutex m;
    static sf::RenderWindow window(sf::VideoMode(800, 600), "Game window");

    std::lock_guard<std::mutex> lock(m);
    return &window;
}

void collide(Mobile* other) {
    std::list<GameObject*> objects = GameObject::getObjects();
    
    EventManager::instance()->raise(new CollisionEvent(globalTime.getTime(), 0.f, 0.f, CollisionEvent::Direction::CLEAR, other, NULL));

    std::list<GameObject*>::iterator object;
    for (object = objects.begin(); object != objects.end(); object++) {
        (*object)->checkCollision(other);
    }

}

std::mutex m;
std::map<sf::Keyboard::Key, bool> keysPressed = { {sf::Keyboard::Key::Space, false},  {sf::Keyboard::Key::A, false}, {sf::Keyboard::Key::D, false}, {sf::Keyboard::Key::Tab, false}, {sf::Keyboard::Key::Escape, false}, {sf::Keyboard::Key::R, false}, {sf::Keyboard::Key::Num1, false}, {sf::Keyboard::Key::Num2, false}, {sf::Keyboard::Key::Num3, false} };
std::list<sf::Keyboard::Key> keys { sf::Keyboard::Key::Space, sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::Tab, sf::Keyboard::Key::Escape, sf::Keyboard::Key::R, sf::Keyboard::Key::Num1, sf::Keyboard::Key::Num2, sf::Keyboard::Key::Num3 };

void checkInputs() {
    std::unique_lock<std::mutex> lock(m);
    if (getWindow()->hasFocus()) {
        lock.unlock();
        for (sf::Keyboard::Key key : keys) {
            bool pressed = keysPressed.at(key);
            if (!pressed && sf::Keyboard::isKeyPressed(key)) {
                EventManager::instance()->raise(new InputEvent(globalTime.getTime(), 0.f, 0.f, key, false));
                keysPressed[key] = true;
            }

            else if (pressed && !sf::Keyboard::isKeyPressed(key)) {
                EventManager::instance()->raise(new InputEvent(globalTime.getTime(), 0.f, 0.f, key, true));
                keysPressed[key] = false;
            }
        }
    }
}

void pauseTime() {
    if (globalTime.isPaused()) {
        globalTime.unpause();
    }

    else {
        globalTime.pause();
    }
}

void timeScale() {
    globalTime.pause();
    if (globalTime.getTic() == 1) {
        globalTime.setTic(2);
    }
    else if (globalTime.getTic() == 2) {
        globalTime.setTic(0.5);
    }
    else if (globalTime.getTic() == 0.5) {
        globalTime.setTic(1);
    }
    globalTime.unpause();
}

void moveCharacter(Mobile* character, float delta_t) {
    character->moveObject(delta_t);
}

void characterThread(Mobile* character, sf::RenderWindow* window) {
    int lastTime = globalTime.getTime();
    ScriptManager scriptManager = ScriptManager();
    dukglue_register_function(scriptManager.getContext(), &checkInputs, "checkInputs");
    dukglue_register_function(scriptManager.getContext(), &collide, "collide");
    dukglue_register_function(scriptManager.getContext(), &moveCharacter, "moveCharacter");
    dukglue_register_method(scriptManager.getContext(), &Mobile::fall, "fall");
    dukglue_register_method(scriptManager.getContext(), &Mobile::jumpVelocity, "jumpVelocity");
    dukglue_register_method(scriptManager.getContext(), &Mobile::inputVelocity, "inputVelocity");

    scriptManager.loadScript("scriptFile.js");

    while (window->isOpen()) {
        float delta_t = (float)(globalTime.getTime() - lastTime);
        lastTime = globalTime.getTime();
        
        scriptManager.runScript("characterFunction", 2, character, delta_t);
    }

}

void parseMessage(char* message, std::map<int, UntexturedPlatformProxy>* platforms, const std::string& filename, int clientNum) {
    char* saveptr = message;
    char* tok = strtok_s(message, " ", &saveptr);
    std::map<int, OtherCharacter*> characters = OtherCharacter::getCharacters();

    while (tok != NULL)
    {
        // Indicates that the next part of the message represents a character
        if (*tok == 'c') {
            int characterId = atoi(strtok_s(NULL, " ", &saveptr));
            float x = atof(strtok_s(NULL, " ", &saveptr));
            float y = atof(strtok_s(NULL, " ", &saveptr));

            if (characters.count(characterId) != 0) {
                characters.at(characterId)->setPosition(x, y);
            }
        }

        else if (*tok == 'n') {
            int characterId = atoi(strtok_s(NULL, " ", &saveptr));

            if (characterId != clientNum) {
                EventManager::instance()->raise(new ConnectEvent(globalTime.getTime(), 0.f, 0.f, characterId));
            }
        }

        else if (*tok == 'd') {
            int characterId = atoi(strtok_s(NULL, " ", &saveptr));

            if (characterId != clientNum) {
                EventManager::instance()->raise(new DisconnectEvent(globalTime.getTime(), 0.f, 0.f, characterId));
            }
        }

        // Indicates that the next part of the message represents a platform
        else if (*tok == 'p') {
            int platformId = atoi(strtok_s(NULL, " ", &saveptr));
            float x = atof(strtok_s(NULL, " ", &saveptr));
            float y = atof(strtok_s(NULL, " ", &saveptr));
            float vx = atof(strtok_s(NULL, " ", &saveptr));
            float vy = atof(strtok_s(NULL, " ", &saveptr));
            float sx = atof(strtok_s(NULL, " ", &saveptr));
            float sy = atof(strtok_s(NULL, " ", &saveptr));
            int r = atoi(strtok_s(NULL, " ", &saveptr));
            int g = atoi(strtok_s(NULL, " ", &saveptr));
            int b = atoi(strtok_s(NULL, " ", &saveptr));
            int a = atoi(strtok_s(NULL, " ", &saveptr));

            if (platforms->count(platformId) == 0) {
                platforms->emplace(platformId, UntexturedPlatformProxy(sf::Vector2f(sx, sy), sf::Vector2f(x, y), sf::Vector2f(vx, vy), sf::Color(r, g, b, a), platformId));
            }
            
            else {
                UntexturedPlatformProxy* platform = &platforms->at(platformId);
                EventManager::instance()->raise(new RepositionEvent(globalTime.getTime(), 0.f, 0.f, platform, sf::Vector2f(x, y)));
                platform->setVelocity(sf::Vector2f(vx, vy));
            }
        }

        tok = strtok_s(NULL, " ", &saveptr);
    }

}

void initializeCharacter(Mobile& character, const Character& init) {
    character = init;
}

Timeline globalTime(NULL, 1);

int main()
{
    // Prepare our context and socket
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");
    
    float clientNum;

    // Send a message to the server to notify it that the client is joining
    zmq::message_t joinMessage(0);
    socket.send(joinMessage, zmq::send_flags::none);
    // Receive a reply from the server containing the client's ID number, and saving that number
    zmq::message_t joinReply;
    socket.recv(joinReply);

    clientNum = *(float*)joinReply.data();

    for (int i = 0; i < clientNum; i++) {
        OtherCharacter::connect(i, "little_dude.png");
    }

    // Create script manager and load it with constructors
    ScriptManager scripter;
    scripter.loadScript("scriptFile.js");

    dukglue_register_constructor<UntexturedPlatform, float, float, float, float>(scripter.getContext(), "UntexturedPlatform");
    dukglue_register_constructor<Character, std::string, float, float>(scripter.getContext(), "Character");
    dukglue_register_function(scripter.getContext(), &initializeCharacter, "initializeCharacter");
    dukglue_register_constructor<SideBoundary, float, float>(scripter.getContext(), "SideBoundary");
    dukglue_register_constructor<SpawnPoint, float, float, float, float, float, float>(scripter.getContext(), "SpawnPoint");
    dukglue_register_constructor<DeathZone, float, float, float, float>(scripter.getContext(), "DeathZone");

    // Create platforms and other game objects
    Mobile character;

    scripter.runScript("createObjects", 1, &character);

    std::map<int, OtherCharacter> otherCharacters;
    std::map<int, UntexturedPlatformProxy> movPlatforms;

    // Create event handlers
    MoveHandler moveHandler;
    DeathHandler deathHandler;
    ScrollHandler scrollHandler;
    CollisionHandler collisionHandler;
    SpawnHandler spawnHandler;
    AgentMoveHandler agentMoveHandler;
    ReplayViewer replayViewer(getWindow());
    InputHandler inputHandler(&character, &replayViewer);
    RepositionHandler repositionHandler;
    ConnectHandler connectHandler("little_dude.png");
    DisconnectHandler disconnectHandler;
    RecordStartHandler recordStartHandler(&replayViewer);
    RecordStopHandler recordStopHandler(&replayViewer);
    DrawHandler drawHandler(getWindow());

    // Create threads
    std::thread charThread(characterThread, &character, getWindow());

    while (getWindow()->isOpen()) {
        sf::Event event;

        // Check for events and handle them
        while (getWindow()->pollEvent(event))
        {
            // Close the window if the user closes the window
            if (event.type == sf::Event::Closed) {
                std::lock_guard<std::mutex> lock(m);
                getWindow()->close();
            }

        }

        EventManager::instance()->raise(new DrawEvent(globalTime.getTime(), 0.f, 0.f));

        // Get character position, store it in a small int array, and send it to the server
        sf::Vector2f characterPosition = character.getPosition();

        float characterPos[3] = { clientNum, characterPosition.x, characterPosition.y };
        zmq::message_t message(3 * sizeof(float));
        memcpy(message.data(), characterPos, 3 * sizeof(float));
        socket.send(message, zmq::send_flags::none);

        zmq::message_t posData;
        socket.recv(posData);
        while (posData.size() == 0) {
            zmq::message_t request(sizeof(float));
            memcpy(request.data(), &clientNum, sizeof(float));
            socket.send(request, zmq::send_flags::none);
            socket.recv(posData);
        }

        parseMessage((char*)posData.data(), &movPlatforms, "little_dude.png", clientNum);

        EventManager::instance()->manage();
    }

    // Join threads
    charThread.join();

    float dcMessage[2] = { clientNum, -1.f };
    zmq::message_t message(2 * sizeof(float));
    memcpy(message.data(), dcMessage, 2 * sizeof(float));
    socket.send(message, zmq::send_flags::none);

    //based on the code from the SFML tutorials at https://www.sfml-dev.org/tutorials/2.5/window-window.php and https://www.sfml-dev.org/tutorials/2.5/graphics-draw.php

    return 0;
}