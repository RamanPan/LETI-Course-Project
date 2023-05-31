#include "food.h"

int Food::counter = 0;

Food::Food(const sf::Vector2f &_position) : Edible("food") {
    initVars();
    position = _position;
    counter++;
    texture_names = { "food" };
    score_modifier = 5;
    initSprite();
}

void Food::toEatenState() {
    Edible::toEatenState();
    counter--;
}