#ifndef WALL_HPP
#define WALL_HPP

#include "../game_tile.h"

class Wall : public GameTile {

public:
    Wall(const sf::Vector2f &, const char &);

    Wall(const Wall &);

    ~Wall();

    Wall &operator=(const Wall &);

    void update(const sf::RenderTarget *, vec3pGT &) override;

    bool isWalkable() const override { return false; }

    char whatIsThis() override { return '@'; }

private:
    char type;

};

#endif