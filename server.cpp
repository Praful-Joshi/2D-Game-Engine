#include "server.hpp"

Server::Server()
        : context(15),
          publisher(context, zmq::socket_type::pub),
          subscriber(context, zmq::socket_type::sub),
          game(2, 1, 1920, 1080, -1) // Initialize game here
{
    subscriber.connect("tcp://localhost:5556");
    subscriber.connect("tcp://localhost:5557");
    subscriber.set(zmq::sockopt::subscribe, "");
    publisher.bind("tcp://*:5555");
    InitializeGame();
}
void Server::GetGameState() {
    while (true) {
        zmq::message_t message;

        // This will block until a message is received
        auto res = subscriber.recv(message, zmq::recv_flags::none);
        if (res) {
            // Convert the message to a string (assuming the server sends string data)
            std::string receivedData(static_cast<char*>(message.data()), message.size());
            // Process the received data (deserialize game state or entities)
            game.deserializeEntitiesServerSide({receivedData}, game);
        } else {
            std::cerr << "Failed to receive message" << std::endl;
        }
        // Optional: break or handle further after processing one message if necessary
        SDL_Delay(2);
    }
}


void Server::PublishGameState() {
    std::vector<std::string> gameState = game.serializeEntitiesServerSide();
    for (size_t i = 0; i < gameState.size(); ++i) {
        zmq::message_t message(gameState[i].size());
        memcpy(message.data(), gameState[i].data(), gameState[i].size());

        // Check if it's the last message part; send it without the 'sndmore' flag
        if (i == gameState.size() - 1) {
            publisher.send(message, zmq::send_flags::none);
        } else {
            publisher.send(message, zmq::send_flags::sndmore);
        }
    }
}


void Server::InitializeGame() {
    Entity* square = new Entity(550, 100, 50, 50, { 255,0,0,255 }, 0, 0, false, true, true, 1, 1);
    game.addEntity(square);

    Entity* square2 = new Entity(650, 100, 50, 50, { 255,0,0,255 }, 0, 0, true, true, true, 3, 2);
    game.addEntity(square2);


    Entity* square3 = new Entity(750, 100, 50, 50, { 255,0,0,255 }, 0, 0, true, true, true, 4, 3);
    game.addEntity(square3);

    Entity* wallLeft = new Entity(500, 700, 500, 500, { 0,0,0,255 }, 0, 0, false, false,false, 2, -1);
    game.addEntity(wallLeft);

    Platform* platform = new Platform(500, 300,150,50,{ 0,0,0,255 }, 0, 1, false, -1,-1, 0, 1000);
    game.addEntity(platform);
}