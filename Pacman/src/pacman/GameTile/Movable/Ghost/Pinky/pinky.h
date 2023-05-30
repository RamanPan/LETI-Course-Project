#ifndef PINKY_HPP
#define PINKY_HPP

#include "../ghost.h"

class Pinky : public Ghost {

public:
    explicit Pinky(const sf::Vector2f &_position) : Ghost("Pinky", _position) {
        scatter_position = {0, 0};
    };

    char whatIsThis() override {
        return 'p';
    }

protected:
    sf::Vector2i getChasePosition() override {
        if (chasing.empty()) {
            std::cout << "ERROR: " + name + " has no target!" << std::endl;
            return home_position;
        }
        Movable *target = nullptr;
        if (chasing.size() == 1) {
            target = chasing.front();
        } else {
            int distance = 9999;
            sf::Vector2i delta = {0, 0};
            for (auto &t: chasing) {
                delta = t->getMapPosition() - getMapPosition();
                int t_distance = delta.x * delta.x + delta.y * delta.y;
                if (distance > t_distance) {
                    distance = t_distance;
                    target = t;
                }
            }
        }

        sf::Vector2i target_tile = target->getMapPosition() + 4 * target->getDirection();

        return target_tile;
    }
};

#endif