#ifndef GHOST_HPP
#define GHOST_HPP

#include "../../../RNG/rng.h"
#include "../movable.h"
#include "../Pacman/pacman.h"

#define NUMBER_BIGGER 9999

class Pacman;

enum GhostStates {
    Frightened, Dead, Chase, Scatter, Escape
};

class Ghost : public Movable {

public:
    Ghost(const std::string &, const sf::Vector2f &);

    Ghost(const Ghost &);

    Ghost &operator=(const Ghost &);

    void update(const sf::RenderTarget *, vec3pGT &) override;

    void render(sf::RenderTarget *) const override;

    inline bool isWalkable() const override { return true; }

    void addChasing(Movable *_target) { chasing.push_back(_target); }

    void toDeadState() { next_state = GhostStates::Dead; };

    void toFrightenedState() { next_state = GhostStates::Frightened; };

    void toChaseState() { next_state = GhostStates::Chase; };

    void toScatterState() { next_state = GhostStates::Scatter; };

    bool isScared() const { return next_state == GhostStates::Frightened; };

    bool isChasing() const { return next_state == GhostStates::Chase; };

    bool isDead() const { return next_state == GhostStates::Dead; };

    std::string getName() const { return name; }

protected:
    std::vector<Movable *> chasing;

    RNG rng;

    GhostStates state, next_state;

    int frightened_timer{}, chase_timer{}, scatter_timer{};

    sf::Sound *sound{};

    const sf::Vector2i home_position = {15, 13}, escape_position = {11, 11};
    sf::Vector2i scatter_position;

    std::string name;

    sf::RectangleShape trail;

    sf::Vector2i trail_position;

    void initVars() override;

    void initTrail();

    void updateTrail();

    void updateAnimation() override;

    void updateMovementDirection(vec3pGT &) override;

    sf::Vector2i chase(vec3pGT &, const sf::Vector2i &) const;

    int rankMove(vec3pGT &, const sf::Vector2i &, const sf::Vector2i &) const;

    sf::Vector2i frightened(vec3pGT &);

    bool canMove(vec3pGT &, const sf::Vector2i &) const;

    void updateState();

    void resetTimers();

    void updateTimers();

    bool isInsideGhostHouse();

    bool isInFrontOfGhostHouse();

    virtual sf::Vector2i getChasePosition() = 0;
};

#endif