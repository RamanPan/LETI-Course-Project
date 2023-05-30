#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "../movable.h"
#include "../../../Config/config.h"
#include "../../Edible/__barrel__.h"
#include "../Ghost/ghost.h"

class Ghost;

enum PacmanState {
    Hurt, Chasing
};

class Pacman : public Movable {
public:
    Pacman(const sf::Vector2f &);

    Pacman(const Pacman &);

    Pacman &operator=(const Pacman &);

    void update(const sf::RenderTarget *, vec3pGT &) override;

    void render(sf::RenderTarget *) const override;

    inline bool isWalkable() const override { return true; }

    void toDeadState();

protected:
    sf::Vector2i next_direction;

    int score;

    sf::Text text;

    PacmanState state, next_state;
    int hurt_timer;

    sf::Sound *sound;

    void interact(vec3pGT &);

    void pollEvents();

    void initVars() override;

    void initText();


    void updateAnimation() override;

    void updateScore();

    void updateMovementDirection(vec3pGT &) override;

    void checkCollisions(const sf::RenderTarget *, vec3pGT &);

    void checkWindowCollisions(const sf::RenderTarget *);

    void collideWithObjects(vec3pGT &);

    void updateState();

    void resetTimer() { hurt_timer = Config::getInstance()->hurt_timer; };

    void updateTimer() {
        if (state == PacmanState::Hurt)
            hurt_timer--;
    }

    char whatIsThis() override {
        return 'o';
    }

private:
    static int counter;
    int index;

};

#endif