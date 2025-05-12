#include "game.hpp"
#include "timeline.hpp"
#include "property.hpp"
#include "init.hpp"
#include "render.hpp"
#include "control.hpp"
#include "movement.hpp"
#include "gravity.hpp"
#include "collisions.hpp"
#include "spawnpoint.hpp"
#include "deathzone.hpp"
#include "sideboundary.hpp"
#include <iostream>
#include <sstream>
#include "peer.hpp"
#include <thread>
#include <atomic>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#endif

#include "platform.hpp"

bool isServer;
int myPlayerPos = 0;
SDL_Color myPlayerColor = { 0,255,255,255 };
int clientID = 0;
bool useEfficientComm = true;
int noplayersJoining = 4;

// Atomic boolean to control the receive thread
std::atomic<bool> receivingData(true);

void receiveDataThread(Peer& peer, Game& game) {
    while (receivingData.load()) {
        std::string receivedData = peer.receive_data();
        if (!receivedData.empty()) {
            if (isServer)
            {
                game.deserializeEntitiesServerSide({ receivedData }, game, peer);
            }
            else
            {
                game.deserializeEntitiesClientSide({ receivedData }, game, peer);
            }
        }
    }
}


void runPeer() {
    Peer peer(isServer, clientID);
    std::cout << "Peer is running\n";

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    Timeline gameTimeline = Timeline();
    Game game = Game(1, 1, 1920, 1080, clientID, useEfficientComm, noplayersJoining);
    game.setTimeline(gameTimeline);

    // Initialize common game entities
    if (isServer)
    {
        std::shared_ptr<Entity> wallLeft = std::make_shared<Entity>(500, 700, 500, 500, SDL_Color{ 0,0,0,255 }, 0, 0, false, false, false, 5, clientID);
        game.addEntity(wallLeft);
        game.addProperty(Property::Collide, 5, std::make_shared<CollisionAttribute>());

        std::shared_ptr<Entity> wallRight = std::make_shared<Entity>(1200, 700, 400, 300, SDL_Color{ 155,230,0,255 }, 0, 0, false, false, false, 6, clientID);
        game.addEntity(wallRight);
        game.addProperty(Property::Collide, 6, std::make_shared<CollisionAttribute>());

        std::shared_ptr<Platform> movingPlatform1 = std::make_shared<Platform>(500, 300, 150, 50, SDL_Color{ 0,0,0,255 }, 0, 1, false, 7, clientID, 500, 900, 0, 0);
        game.addEntity(movingPlatform1);
		game.addProperty(Property::Collide, 7, std::make_shared<CollisionAttribute>());
		game.addProperty(Property::Move, 7, std::make_shared<MovementAttribute>());

        std::shared_ptr<Platform> movingPlatform2 = std::make_shared<Platform>(1500, 300, 150, 50, SDL_Color{ 100,100,20,255 }, 1, 0, false, 8, clientID, 0, 0, 300, 600);
        game.addEntity(movingPlatform2);
        game.addProperty(Property::Collide, 8, std::make_shared<CollisionAttribute>());
        game.addProperty(Property::Move, 8, std::make_shared<MovementAttribute>());
    }

	//initialize player specific entities
    std::shared_ptr<Entity> player = std::make_shared<Entity>(myPlayerPos, 100, 50, 50, myPlayerColor, 0, 0, true, true, true, clientID, clientID);
    game.addEntity(player);
    game.addProperty(Property::Collide, clientID, std::make_shared<CollisionAttribute>());
    game.addProperty(Property::Control, clientID, std::make_shared<ControlAttribute>());
    game.addProperty(Property::Gravity, clientID, std::make_shared<GravityAttribute>());
    game.addProperty(Property::Move, clientID, std::make_shared<MovementAttribute>());

    if (isServer)
    {
		game.updatePreviousEntityStates(game);
    }

    // Start a separate thread for receiving data
    std::thread receiveThread(receiveDataThread, std::ref(peer), std::ref(game));

    // Initialize the engine
    if (!init(window, renderer)) {
        std::cerr << "Failed to initialize the engine." << std::endl;
        return;
    }

    // Run the game loop
    bool quit = false;
    SDL_Event e;


    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT || e.type == SDL_WINDOWEVENT_CLOSE) {
                quit = true;
                if (!isServer)
                {
                    player->setClientID(-1);
                    std::vector<std::string> temp = game.serializeEntitiesClientSide(game);
                    std::string debugMessage = "sent data size: " + std::to_string(temp.size()) + "\n";
                    // OutputDebugString(debugMessage.c_str());  // This will print in the Debug window
                    if(temp.size() > 0) peer.send_data(temp);
                }
                else
                {
                    player->setClientID(-2);
                    std::vector<std::string> temp = game.serializeEntitiesServerSide(game);
                    std::string debugMessage = "sent data size: " + std::to_string(temp.size()) + "\n";
                    // OutputDebugString(debugMessage.c_str());  // This will print in the Debug window
                    if (temp.size() > 0) peer.send_data(temp);
                }
                game.removeEntity(player);
            }
            else if (e.type == SDL_WINDOWEVENT) {
                if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    // Window size has changed, update the window size in the engine
                    updateWindowSize(window, game.getCurrentWidth(), game.getCurrentHeight());
                }
            }
        }
        //send and receive data
        if (isServer) {
            std::vector<std::string> temp = game.serializeEntitiesServerSide(game);
            // Prepare the message to print to the debug window
            std::string debugMessage = "sent data size: " + std::to_string(temp.size()) + "\n";
            // OutputDebugString(debugMessage.c_str());  // This will print in the Debug window
            if (temp.size() > 0) peer.send_data(temp);
        }
        else {
            std::vector<std::string> temp = game.serializeEntitiesClientSide(game);
            std::string debugMessage = "sent data size: " + std::to_string(temp.size()) + "\n";
            // OutputDebugString(debugMessage.c_str());  // This will print in the Debug window
            if (temp.size() > 0) peer.send_data(temp);
        }
        // Update and render the game
        game.updateProperties(Property::Control, &gameTimeline);
        game.updateProperties(Property::DeathZone, &gameTimeline);
        game.updateProperties(Property::SpawnPoint, &gameTimeline);
        game.updateProperties(Property::SideBoundary, &gameTimeline);
        game.updateProperties(Property::Gravity, &gameTimeline);
        game.updateProperties(Property::Collide, &gameTimeline);
        game.updateProperties(Property::Move, &gameTimeline);
        clearScreen(renderer);
        renderEntities(renderer, game);
        presentScreen(renderer);
        SDL_Delay(2);
    }

    // Stop receiving thread and wait for it to finish
    receivingData.store(false);
    if (receiveThread.joinable()) {
        receiveThread.join();
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <server|client|peer> [bindAddress] [connectAddress]" << std::endl;
        return EXIT_FAILURE;
    }

    std::string mode = argv[1];
    try {
        if (mode == "masterPeer") {
            isServer = true;
            myPlayerPos = 500;
			clientID = 1;
			myPlayerColor = { 0,255,255,255 };
            runPeer();
        }
        else if (mode == "peer2") {
            isServer = false;
            myPlayerPos = 800;
			clientID = 2;
            myPlayerColor = { 255,165,0,255 };
            runPeer();
        }
        else if (mode == "peer3") {
            isServer = false;
            myPlayerPos = 1200;
            clientID = 3;
            myPlayerColor = { 255,0,255,255 };
            runPeer();
        }
        else if (mode == "peer4") {
            isServer = false;
            myPlayerPos = 1500;
            clientID = 4;
            myPlayerColor = { 255,255,165,255 };
            runPeer();
        }
		else if (mode == "peer5") {
			isServer = false;
			myPlayerPos = 1800;
			clientID = 5;
			myPlayerColor = { 0,255,0,255 };
			runPeer();
		}
		else if (mode == "peer6") {
			isServer = false;
			myPlayerPos = 2100;
			clientID = 6;
			myPlayerColor = { 255,0,0,255 };
			runPeer();
		}
		else if (mode == "peer7") {
			isServer = false;
			myPlayerPos = 2400;
			clientID = 7;
			myPlayerColor = { 0,0,255,255 };
			runPeer();
		}
		else if (mode == "peer8") {
			isServer = false;
			myPlayerPos = 2700;
			clientID = 8;
			myPlayerColor = { 255,255,0,255 };
			runPeer();
		}
		else if (mode == "peer9") {
			isServer = false;
			myPlayerPos = 3000;
			clientID = 9;
			myPlayerColor = { 255,0,255,255 };
			runPeer();
		}
		else if (mode == "peer10") {
			isServer = false;
			myPlayerPos = 3300;
			clientID = 10;
			myPlayerColor = { 0,255,255,255 };
			runPeer();
		}
        else {
            std::cerr << "Unknown mode: " << mode << std::endl;
            return EXIT_FAILURE;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}