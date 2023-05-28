#ifndef WINDOW_TITLE_HPP
#define WINDOW_TITLE_HPP

#include "../Config/config.h"

class WindowTitle {

public:
    WindowTitle(const std::string&, const sf::Vector2f&);
    ~WindowTitle();

    void update();

    void render(sf::RenderTarget*) const;

private:
    std::string content;
    sf::Text text;

};

#endif