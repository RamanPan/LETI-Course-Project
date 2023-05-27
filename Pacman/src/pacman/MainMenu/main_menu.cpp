#include <fstream>
#include "main_menu.h"
#include "../Game/game.h"

MainMenu::MainMenu() {
    selection_index = 0;
    conf->mainMenuFlag = false;
    initWindow();
    initText();
    initSounds();
    animation = new MenuAnimation({200.f, 145.f});
    press_start->play();

}

MainMenu::~MainMenu() {
    press_start->stop();
    delete window;
    delete title;
    for (MenuEntry *entry: options) delete entry;
}

void MainMenu::pollEvents() {
    sf::Event event{};
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                if (selection_index > 0) selection_index--;
            } else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                if (selection_index < 2) selection_index++;
            } else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                okey->play();
                while (okey->getStatus() == sf::Sound::Status::Playing) {
                    sf::sleep(sf::milliseconds(10));
                }
                switch (selection_index) {
                    case 0:
                        conf->selected_map = conf->maps["single-player"];
                        conf->score = 0;
                        press_start->stop();
                        window->close();
                        sf::sleep(sf::milliseconds(500));
                        Game().run();
                        break;
                    case 1:
                        conf->selected_map = conf->maps["saved-player"];
                        {
                            std::ifstream in(conf->pathToSaveAndReadScore,
                                             std::ios_base::in);
                            in >> conf->score;
                            in.close();
                        }
                        press_start->stop();
                        window->close();
                        sf::sleep(sf::milliseconds(500));
                        Game().run();
                        break;
                    case 2:
                        exit(1);
                }
            }
        }
    }


}

void MainMenu::update() {
    options[selection_index]->setActive(false);
    pollEvents();
    options[selection_index]->setActive(true);
    title->update();
    for (MenuEntry *entry: options) entry->update();
    animation->update();
}

void MainMenu::render() const {
    window->clear();
    // Render menu UI.
    animation->render(window);
    title->render(window);
    for (auto &entry: options) entry->render(window);
    window->display();
}

void MainMenu::initWindow() {
    const sf::VideoMode videoMode = sf::VideoMode(1000, 600);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;
    window = new sf::RenderWindow(videoMode, conf->window_title, style);
    window->setFramerateLimit(conf->window_framerate);
    window->setVerticalSyncEnabled(true);
    window->setIcon(conf->icon->getSize().x, conf->icon->getSize().y, conf->icon->getPixelsPtr());
    window->setVisible(true);

}

void MainMenu::initText() {
    title = new WindowTitle(conf->window_title, {260.f, 150.f});
    options[0] = new MenuEntry("         START", {200.f, 260.f});
    options[1] = new MenuEntry("         LOAD LAST SAVE", {200.f, 300.f});
    options[2] = new MenuEntry("         EXIT", {200.f, 340.f});
}

void MainMenu::initSounds() {
    press_start = conf->sounds["start"];
    okey = conf->sounds["choice"];
}

void MainMenu::show() {
    while (isRunning()) {
        update();
        render();
    }
}

