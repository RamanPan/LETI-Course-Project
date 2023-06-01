#ifndef PAUSE_MENU_HPP
#define PAUSE_MENU_HPP

#include "../Config/config.h"
#include "../MainMenu/MenuEntry/menu_entry.h"
#include "../WindowTitle/window_title.h"
#include "../MainMenu/MenuAnimation/menu_animation.h"

class PauseMenu {

public:
    PauseMenu();

    ~PauseMenu();

    void show();

private:
    Config *conf = Config::getInstance();
    sf::RenderWindow *window;
    MenuAnimation *animation;
    sf::Sound *okey;
    WindowTitle *title;
    MenuEntry *options[4];
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