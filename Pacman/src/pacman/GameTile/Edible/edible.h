#ifndef EDIBLE_HPP
#define EDIBLE_HPP

#include "../game_tile.h"

enum EdibleState {
    Active, Eaten
};

class Edible : public GameTile {

public:
    explicit Edible(const std::string &);

    Edible(const Edible &);

    ~Edible() override;

    Edible &operator=(const Edible &);

    void update(const sf::RenderTarget *, vec3pGT &) override;

    inline bool isWalkable() const override { return true; }

    virtual void toEatenState();

    sf::Sound *sound;

    int getScoreModifier() const { return score_modifier; }

    bool isEaten() { return state == EdibleState::Eaten; }

protected:
    EdibleState state;
    int score_modifier;

};

#endif