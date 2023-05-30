#include "movable.h"

Movable::Movable() = default;

Movable::Movable(const Movable &_movable) { *this = _movable; }

Movable::~Movable() = default;

Movable &Movable::operator=(const Movable &_other) {
    this->GameTile::operator=(_other);
    direction = _other.direction;
    speed = _other.speed;
    return *this;
}

void Movable::initVars() {
    GameTile::initVars();
    direction = Directions::None;
    speed = Config::getInstance()->speed;
}
