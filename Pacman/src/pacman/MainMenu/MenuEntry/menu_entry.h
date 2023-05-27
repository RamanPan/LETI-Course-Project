#ifndef MENU_ENTRY_HPP
#define MENU_ENTRY_HPP

#include "../../Config/config.h"

class MenuEntry {

public:
    MenuEntry(const std::string &, const sf::Vector2f &);

    ~MenuEntry();

    void update();

    void render(sf::RenderTarget *) const;

    void setActive(bool);

private:
    std::string content;
    sf::Text text;
    bool is_active;

};

#endif