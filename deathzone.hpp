#ifndef DEATH_ZONE_ATTRIBUTE_HPP
#define DEATH_ZONE_ATTRIBUTE_HPP

#include "attribute.hpp"
#include "eventmanager.hpp"
#include "game.hpp"
#include <iostream>

class DeathZoneAttribute : public Attribute {
public:
    DeathZoneAttribute(int spawnId, const std::unordered_map<int, SDL_Rect>& initialPositions)
        : spawnPointId(spawnId), originalPositions(initialPositions) {}
    void update(Entity& entity, Game& game) override;
private:
    int spawnPointId;
    std::unordered_map<int, SDL_Rect> originalPositions;
};

#endif // DEATH_ZONE_ATTRIBUTE_HPP