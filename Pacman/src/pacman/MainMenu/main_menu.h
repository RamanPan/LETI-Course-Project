#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "../Config/config.h"
#include "MenuEntry/menu_entry.h"
#include "../WindowTitle/window_title.h"
#include "MenuAnimation/menu_animation.h"

class MainMenu {

public:
    MainMenu();

    ~MainMenu();

    void show();

private:
    sf::RenderWindow *window;
    MenuAnimation *animation;
    sf::Sound *press_start, *okey;
    WindowTitle *title;
    Config *conf = Config::getInstance();
    MenuEntry *options[3];
    int selection_index;

    void pollEvents();

    void update();

    void render() const;

    bool isRunning() const {
        if (window == nullptr) return false;
        return window->isOpen();
    }

    void initWindow();

    void initText();

    void initSounds();

};

#endif