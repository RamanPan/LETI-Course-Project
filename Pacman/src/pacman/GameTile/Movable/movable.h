#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include <cmath>

#include "../../Config/config.h"
#include "../game_tile.h"

namespace Directions {
    static const sf::Vector2i Up = {0, -1};
    static const sf::Vector2i Down = {0, 1};
    static const sf::Vector2i Left = {-1, 0};
    static const sf::Vector2i Right = {1, 0};
    static const sf::Vector2i None = {0, 0};
}

class Movable : public GameTile {
public:
    Movable();

    Movable(const Movable &);

    virtual ~Movable();

    Movable &operator=(const Movable &);

    sf::Vector2i getDirection() { return direction; }

protected:
    sf::Vector2i direction;

    float speed;

    virtual void initVars();

    virtual void updateMovementDirection(vec3pGT &) = 0;

    virtual void updateAnimation() = 0;
};

#endif