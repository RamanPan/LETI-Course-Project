#ifndef GAME_HPP
#define GAME_HPP

#include "../GameTile/__barrel__.h"
#include "../Config/config.h"
#include "../WindowTitle/window_title.h"

#define MAP_ROWS 31
#define MAP_COLUMNS 28

class Game {

public:
    Game();

    ~Game();

    void run();

private:
    sf::RenderWindow *window{};
    sf::Sound *ty_sound{}, *background_music{};
    vec3pGT *map{};
    Config *conf = Config::getInstance();
    WindowTitle *title{}, *game_over_title{};
    bool game_over = false;

    void pollEvents();

    void update();

    void render() const;

    void save();

    bool isRunning() const;

    void initWindow();

    void initMap();

    vector<vector<char>> readMap();

    void initTitles();

    void initSounds();

};

#endif