// movement_core/attribute.hpp
#ifndef MOVEMENT_ATTRIBUTE_HPP
#define MOVEMENT_ATTRIBUTE_HPP

#include "attribute.hpp"
#include "entity.hpp"
#include "platform.hpp"
#include "game.hpp"

class MovementAttribute : public Attribute {
public:
    void update(Entity& entity, Game& game) override;
};

#endif  // MOVEMENT_ATTRIBUTE_HPP
