#include <fstream>
#include "pause_menu.h"

PauseMenu::PauseMenu() {
    selection_index = 0;
    Config::getInstance()->window_title = "Pacman want rest";
    initWindow();
    initText();
    initSounds();
    animation = new MenuAnimation({130.f, 145.f});
}

PauseMenu::~PauseMenu() {
    conf->window_title = "Pacman for kursach";
    delete window;
    delete title;
    for (MenuEntry *entry: options) delete entry;
}

void PauseMenu::pollEvents() {
    sf::Event event{};
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                if (selection_index > 0) selection_index--;
            } else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                if (selection_index < 3) selection_index++;
            } else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                okey->play();
                while (okey->getStatus() == sf::Sound::Status::Playing) {
                    sf::sleep(sf::milliseconds(10));
                }
                switch (selection_index) {
                    case 0:
                        window->close();
                        break;
                    case 1:
                        conf->flagSave = true;
                        window->close();
                        break;
                    case 2:
                        conf->loadFlag = true;
                        {
                            std::ifstream in(conf->pathToSaveAndReadScore,
                                             std::ios_base::in);
                            in >> conf->score;
                            in.close();
                        }
                        conf->selected_map = conf->maps["saved-player"];
                        window->close();
                        break;
                    case 3:
                        window->close();
                        conf->mainMenuFlag = true;
                        break;
                }
            }
        }
    }
}

void PauseMenu::update() {
    options[selection_index]->setActive(false);
    pollEvents();
    options[selection_index]->setActive(true);
    title->update();
    for (MenuEntry *entry: options) entry->update();
    animation->update();
}

void PauseMenu::render() const {
    window->clear();
    animation->render(window);
    title->render(window);
    for (auto &entry: options) entry->render(window);
    window->display();

}

void PauseMenu::initWindow() {
    const sf::VideoMode videoMode = sf::VideoMode(800, 600);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;
    window = new sf::RenderWindow(videoMode, conf->pause_window_title, style);
    window->setFramerateLimit(conf->window_framerate);
    window->setVerticalSyncEnabled(true);
    window->setIcon(conf->icon->getSize().x, conf->icon->getSize().y, conf->icon->getPixelsPtr());
    window->setVisible(true);
}

void PauseMenu::initText() {
    title = new WindowTitle(conf->window_title, {200.f, 150.f});
    options[0] = new MenuEntry("CONTINUE", {300.f, 260.f});
    options[1] = new MenuEntry("SAVE GAME", {300.f, 300.f});
    options[2] = new MenuEntry("LOAD SAVE", {300.f, 340.f});
    options[3] = new MenuEntry("EXIT TO MENU", {300.f, 380.f});
}

void PauseMenu::initSounds() {
    okey = conf->sounds["choice"];
}

void PauseMenu::show() {
    while (isRunning()) {
        update();
        render();
    }
}
