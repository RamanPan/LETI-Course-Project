#ifndef FRUIT_HPP
#define FRUIT_HPP

#include <SFML/Graphics.hpp>
#include <random>

#include "../edible.h"

class Fruit : public Edible {
public:
    explicit Fruit(const sf::Vector2f &);

    char whatIsThis() override {
        return 'f';
    }

};

#endif