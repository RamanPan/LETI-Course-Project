#ifndef FOOD_HPP
#define FOOD_HPP

#include <SFML/Graphics.hpp>
#include "../edible.h"

class Food : public Edible {

public:
    explicit Food(const sf::Vector2f &);

    void toEatenState() override;

    char whatIsThis() { return '*'; }

    static int counter;
};

#endif