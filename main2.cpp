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
#include "platform.hpp"
#include "control_handler.hpp"
#include "collision_handler.hpp"
#include "death_handler.hpp"
#include "sideboundary_handler.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int BUBBLE_SIZE = 40;
const int CANNON_WIDTH = 60;
const int CANNON_HEIGHT = 20;
const int BULLET_SIZE = 20;

enum BubbleColor { RED, BLUE, GREEN };

void createBubbleGrid(Game& game) {
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < SCREEN_WIDTH / BUBBLE_SIZE; ++col) {
            BubbleColor color = static_cast<BubbleColor>(rand() % 3);
            SDL_Color sdlColor;
            switch (color) {
            case RED: sdlColor = { 255, 0, 0, 255 }; break;
            case BLUE: sdlColor = { 0, 0, 255, 255 }; break;
            case GREEN: sdlColor = { 0, 255, 0, 255 }; break;
            }
            auto bubble = std::make_shared<Entity>(col * BUBBLE_SIZE, row * BUBBLE_SIZE, BUBBLE_SIZE, BUBBLE_SIZE, sdlColor, 0, 0, false, false, false, 1, 1);
            game.addEntity(bubble);
            game.addProperty(Property::Collide, bubble->getEntityID(), std::make_shared<CollisionAttribute>());
        }
    }
}

int main(int argc, char* args[]) {
    srand(static_cast<unsigned>(time(nullptr)));

    Timeline gameTimeline = Timeline();
    Game game = Game(1, 1, 1920, 1080, 1, false, 1);
    game.setTimeline(gameTimeline);

    EventManager eventManager;
    CollisionHandler collisionHandler;
    ControlHandler   controlHandler;
    game.setEventManager(eventManager);

    // Register the handlers for events
    eventManager.registerHandler(EventType::COLLISION, &collisionHandler);
    eventManager.registerHandler(EventType::CONTROL, &controlHandler);

    SDL_Window* window = SDL_CreateWindow("Bubble Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    createBubbleGrid(game);

    // Create cannon
    auto cannon = std::make_shared<Entity>(SCREEN_WIDTH / 2 - CANNON_WIDTH / 2, SCREEN_HEIGHT - CANNON_HEIGHT, CANNON_WIDTH, CANNON_HEIGHT, SDL_Color{ 200, 200, 200, 255 }, 0, 0, false, true, true, 2, 1);
    game.addEntity(cannon);
    game.addProperty(Property::Control, cannon->getEntityID(), std::make_shared<ControlAttribute>());
    game.addProperty(Property::Move, cannon->getEntityID(), std::make_shared<MovementAttribute>());

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    cannon->setVelocityX(-5);
                    break;
                case SDLK_RIGHT:
                    cannon->setVelocityX(5);
                    break;
                case SDLK_SPACE:
                    // Shoot bubble
                    BubbleColor color = static_cast<BubbleColor>(rand() % 3);
                    SDL_Color bulletColor;
                    switch (color) {
                    case RED: bulletColor = { 255, 0, 0, 255 }; break;
                    case BLUE: bulletColor = { 0, 0, 255, 255 }; break;
                    case GREEN: bulletColor = { 0, 255, 0, 255 }; break;
                    }
                    auto bullet = std::make_shared<Entity>(cannon->getRect().x + CANNON_WIDTH / 2 - BULLET_SIZE / 2, cannon->getRect().y - BULLET_SIZE, BULLET_SIZE, BULLET_SIZE, bulletColor, 0, -5, false, true, false, 3, 1);
                    game.addEntity(bullet);
                    game.addProperty(Property::Move, bullet->getEntityID(), std::make_shared<MovementAttribute>());
                    game.addProperty(Property::Collide, bullet->getEntityID(), std::make_shared<CollisionAttribute>());
                    break;
                }
            }
            else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) {
                    cannon->setVelocityX(0);
                }
            }
        }

        game.updateProperties(Property::Move, game.getTimeline());
        game.updateProperties(Property::Control, game.getTimeline());
        game.updateProperties(Property::Collide, game.getTimeline());

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (const auto& [entityID, entity] : game.getEntities()) {
            SDL_SetRenderDrawColor(renderer, entity->getColor().r, entity->getColor().g, entity->getColor().b, entity->getColor().a);
            SDL_Rect rect = entity->getRect();
            SDL_RenderFillRect(renderer, &rect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}