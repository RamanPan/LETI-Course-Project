#ifndef MENU_ANIMATION_HPP
#define MENU_ANIMATION_HPP

#include "../../Config/config.h"

class MenuAnimation {

public:
    explicit MenuAnimation(const sf::Vector2f &);

    ~MenuAnimation();

    void update();

    void render(sf::RenderTarget *) const;

private:
    sf::Sprite sprite;
    std::vector<sf::Texture *> textures;
    int frame_counter;
    const int animation_fps = 8;
};

#endif