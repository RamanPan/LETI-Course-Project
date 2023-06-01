#include <fstream>
#include "game.h"
#include "../MainMenu/main_menu.h"

Game::Game() {
    initSounds();
    Config::getInstance()->loadFlag = false;
    background_music->play();
    initMap();
    initWindow();
}

Game::~Game() {
    ty_sound->play();
    while (ty_sound->getStatus() == sf::Sound::Status::Playing) {
        sf::sleep(sf::milliseconds(100.f));
    }

    background_music->stop();
    delete window;
    for (vec2pGT &vec2: *map) {
        for (vec1pGT &vec1: vec2) {
            for (GameTile *tile: vec1) {
                delete tile;
            }
        }
    }
}

void Game::pollEvents() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }
        if (game_over && event.type == sf::Event::KeyPressed) {
            window->close();
        }
    }
}

void Game::update() {
    pollEvents();
    if (!game_over) {
        for (vec2pGT &vec2: *map) {
            for (vec1pGT &vec1: vec2) {
                for (GameTile *tile: vec1) {
                    if (tile != nullptr) {
                        tile->update(window, *map);
                    }
                }
            }
        }
        if (Food::counter <= 0) game_over = true;
    } else {
        sf::sleep(sf::milliseconds(200));
    }
}

void Game::render() const {
    window->clear();
    title->render(window);
    for (const vec2pGT &vec2: *map) {
        for (const vec1pGT &vec1: vec2) {
            for (GameTile *tile: vec1) {
                if (tile != nullptr) tile->render(window);
            }
        }
    }
    if (game_over) game_over_title->render(window);
    window->display();
}

bool Game::isRunning() const {
    if (window == nullptr) return false;
    return window->isOpen();
}

void Game::initWindow() {
    conf->window_size.x = conf->map_size.x * conf->tile_size + conf->offset.x * 2;
    conf->window_size.y = conf->map_size.y * conf->tile_size + conf->offset.y * 2;
    const sf::VideoMode videoMode = sf::VideoMode(conf->window_size.x, conf->window_size.y);
    const sf::Uint32 style = sf::Style::Close | sf::Style::Titlebar;
    window = new sf::RenderWindow(videoMode, conf->window_title, style);
    window->setFramerateLimit(conf->window_framerate);
    window->setVerticalSyncEnabled(true);
    window->setIcon(conf->icon->getSize().x, conf->icon->getSize().y, conf->icon->getPixelsPtr());
    initTitles();
    window->setVisible(true);
}

vector<vector<char>> Game::readMap() {
    std::ifstream file(conf->selected_map, std::ios::binary);
    std::string line;
    unsigned long int line_count = 0, line_length = 0;
    vector<vector<char>> char_map = {};
    if (!file.is_open()) {
        std::cout << "ERROR: Cannot find map file" << std::endl;
        abort();
    }
    while (std::getline(file, line)) {
        if (line.size() > line_length) line_length = line.size();
        vector<char> temp = {};
        for (char &i: line) {
            temp.push_back(i);
        }
        char_map.push_back(temp);
        line_count++;
    }
    conf->map_size.x = line_length;
    conf->map_size.y = line_count;
    return char_map;
}

void Game::initMap() {
    map = new vec3pGT;
    conf->loadFlag = false;
    vector<vector<char>> char_map = readMap();
    std::vector<Ghost *> ghosts[2];
    Pacman *pacmans[2];
    int i = 0, j = 0;
    for (auto &line: char_map) {
        vec2pGT temp = {};
        for (char &c: line) {
            sf::Vector2f position = {
                    i * conf->tile_size,
                    j * conf->tile_size,
            };
            GameTile *tile = nullptr;
            if (std::string("123456!@#$%^").find(c) != std::string::npos) {
                tile = new Wall(position, c);
            } else if (c == '*') {
                tile = new Food(position);
            } else if (c == '0') {
                tile = new PowerPellet(position);
            } else if (c == 'f') {
                tile = new Fruit(position);
            } else {
                int index = islower(c) == 0 ? 1 : 0;
                if (c == 'O' || c == 'o') {
                    tile = new Pacman(position);
                    pacmans[index] = (Pacman *) tile;
                } else if (c == 'B' || c == 'b') {
                    tile = new Blinky(position);
                    ghosts[index].push_back((Ghost *) tile);
                } else if (c == 'C' || c == 'c') {
                    tile = new Clyde(position);
                    ghosts[index].push_back((Ghost *) tile);
                } else if (c == 'I' || c == 'i') {
                    tile = new Inky(position);
                    ghosts[index].push_back((Ghost *) tile);
                } else if (c == 'P' || c == 'p') {
                    tile = new Pinky(position);
                    ghosts[index].push_back((Ghost *) tile);
                }
            }
            temp.push_back({tile});
            i++;
        }
        j++;
        i = 0;
        map->push_back(temp);
    }

    for (int i = 0; i < 2; i++) {
        for (Ghost *g: ghosts[i]) {
            g->addChasing(pacmans[i]);
            if (g->getName() == "Inky") {
                for (Ghost *b: ghosts[i]) {
                    if (b->getName() == "Blinky") {
                        g->addChasing(b);
                    }
                }
            }
        }
    }
}

void Game::initTitles() {
    const sf::Vector2f title_position = {
            static_cast<float>(conf->window_size.x) / 2 - 100,
            static_cast<float>(conf->offset.y) / 2 - 15,
    };
    title = new WindowTitle(conf->window_title, title_position);

    const sf::Vector2f go_title_position = {
            static_cast<float>(conf->window_size.x) / 2 - 135,
            static_cast<float>(conf->window_size.y) / 2 - 40,
    };
    game_over_title = new WindowTitle("You win!\n Congratulate!", go_title_position);
}

void Game::initSounds() {
    ty_sound = conf->sounds["exit"];
    background_music = conf->sounds["background"];
    background_music->setVolume(50.f);
}

void Game::save() {
    std::ofstream outStream(conf->pathToSaveAndReadScore,
                            std::ios_base::out);
    outStream << conf->score;
    outStream.close();
    std::ofstream out(conf->pathToSave,
                      std::ios_base::out);
    char mapForSave[MAP_ROWS][MAP_COLUMNS];
    for (int i = 0; i < MAP_ROWS; ++i)
        for (int j = 0; j < MAP_COLUMNS; ++j)
            mapForSave[i][j] = ' ';
    int x, y;
    for (const vec2pGT &vec2: *map) {
        for (const vec1pGT &vec1: vec2) {
            for (GameTile *tile: vec1) {
                if (tile != nullptr && !tile->eat) {
                    y = tile->getMapPosition().y;
                    x = tile->getMapPosition().x;
                    if (tile->whatIsThis() == '@') mapForSave[y][x] = tile->texture_names.at(0).at(0);
                    else mapForSave[y][x] = tile->whatIsThis();
                }
            }
        }
    }
    for (int i = 0; i < MAP_ROWS; ++i) {
        for (int j = 0; j < MAP_COLUMNS; ++j) {
            out << mapForSave[i][j];
        }
        out << std::endl;
    }
}

void Game::run() {
    while (isRunning()) {
        update();
        render();
        if (conf->flagSave) {
            conf->flagSave = false;
            save();
        } else if (conf->mainMenuFlag) {
            background_music->stop();
            if (window != nullptr) window->close();
            sf::sleep(sf::milliseconds(500));
            MainMenu().show();
        } else if (conf->loadFlag) {
            for (vec2pGT &vec2: *map) {
                for (vec1pGT &vec1: vec2) {
                    for (GameTile *tile: vec1) {
                        delete tile;
                    }
                }
            }
            conf->loadFlag = false;
            initMap();
        }
    }
}

