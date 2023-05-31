#ifndef POWER_PELLET_HPP
#define POWER_PELLET_HPP

#include <SFML/Graphics.hpp>

#include "../edible.h"

class PowerPellet : public Edible {
public:
    explicit PowerPellet(const sf::Vector2f &);

    char whatIsThis() override { return '0'; }

};

#endif