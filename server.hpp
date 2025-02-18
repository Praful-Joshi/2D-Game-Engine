#ifndef SERVER
#define SERVER
#include "game.hpp"
#include "platform.hpp"
#include <string>
#include <zmq.hpp>
#include <iostream>
#include "init.hpp"
#include "render.hpp"
#include "gravity.hpp"
#include "scaling.hpp"
#include "movement.hpp"
#include "timeline.hpp"
#include "collisions.hpp"

#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#if (defined (WIN32))
#include <zhelpers.hpp>
#endif

class Server{
    public:
    Server();
    void PublishGameState();
    void InitializeGame();
    void GetGameState();
    Game game;

    private:
    // Server Sockets
    zmq::context_t context;
    zmq::socket_t  publisher;
    zmq::socket_t  subscriber;

};

#endif
