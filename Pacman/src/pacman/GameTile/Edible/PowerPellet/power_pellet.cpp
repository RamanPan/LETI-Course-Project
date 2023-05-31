#include "power_pellet.h"

PowerPellet::PowerPellet(const sf::Vector2f &_position) : Edible("pallets") {
    initVars();
    position = _position;
    texture_names = {"power_pellet"};
    score_modifier = 0;
    initSprite();
}