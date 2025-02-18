#include "game.hpp"
#include "entity.hpp"
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
const int PLAYER_WIDTH = 60;
const int PLAYER_HEIGHT = 20;
const int INVADER_SIZE = 40;
const int BULLET_SIZE = 10;

void createInvaders(Game& game) {
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < SCREEN_WIDTH / (INVADER_SIZE * 2); ++col) {
            auto invader = std::make_shared<Entity>(col * INVADER_SIZE * 2, row * INVADER_SIZE * 2, INVADER_SIZE, INVADER_SIZE, SDL_Color{ 0, 255, 0, 255 }, 1, 0, false, true, false, 1, 1);
            game.addEntity(invader);
            game.addProperty(Property::Move, invader->getEntityID(), std::make_shared<MovementAttribute>());
            game.addProperty(Property::Collide, invader->getEntityID(), std::make_shared<CollisionAttribute>());
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

    SDL_Window* window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    createInvaders(game);

    // Create player
    auto player = std::make_shared<Entity>(SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2, SCREEN_HEIGHT - PLAYER_HEIGHT * 2, PLAYER_WIDTH, PLAYER_HEIGHT, SDL_Color{ 200, 200, 200, 255 }, 0, 0, false, true, true, 2, 1);
    game.addEntity(player);
    game.addProperty(Property::Control, player->getEntityID(), std::make_shared<ControlAttribute>());
    game.addProperty(Property::Move, player->getEntityID(), std::make_shared<MovementAttribute>());

    SDL_Event e;
    bool quit = false;
    Uint32 lastTime = SDL_GetTicks();
    Uint32 invaderMoveTime = 0;

    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        Uint32 deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    player->setVelocityX(-5);
                    break;
                case SDLK_RIGHT:
                    player->setVelocityX(5);
                    break;
                case SDLK_SPACE:
                    // Shoot bullet
                    auto bullet = std::make_shared<Entity>(player->getRect().x + PLAYER_WIDTH / 2 - BULLET_SIZE / 2, player->getRect().y - BULLET_SIZE, BULLET_SIZE, BULLET_SIZE, SDL_Color{ 255, 255, 255, 255 }, 0, -10, false, true, false, 3, 1);
                    game.addEntity(bullet);
                    game.addProperty(Property::Move, bullet->getEntityID(), std::make_shared<MovementAttribute>());
                    game.addProperty(Property::Collide, bullet->getEntityID(), std::make_shared<CollisionAttribute>());
                    break;
                }
            }
            else if (e.type == SDL_KEYUP) {
                if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) {
                    player->setVelocityX(0);
                }
            }
        }

        game.updateProperties(Property::Move, game.getTimeline());
        game.updateProperties(Property::Control, game.getTimeline());
        game.updateProperties(Property::Collide, game.getTimeline());

        // Move invaders
        invaderMoveTime += deltaTime;
        if (invaderMoveTime >= 1000) {
            invaderMoveTime = 0;
            bool moveDown = false;
            for (const auto& [entityID, entity] : game.getEntities()) {
                if (entity->getVelocityX() != 0) {
                    if (entity->getRect().x <= 0 || entity->getRect().x + entity->getRect().w >= SCREEN_WIDTH) {
                        moveDown = true;
                        break;
                    }
                }
            }
            for (const auto& [entityID, entity] : game.getEntities()) {
                if (entity->getVelocityX() != 0) {
                    if (moveDown) {
                        entity->setVelocityX(-entity->getVelocityX());
                        entity->setRectY(entity->getRect().y + INVADER_SIZE / 2);
                    }
                }
            }
        }

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