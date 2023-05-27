#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <vector>

#define FRIGHTENED_TIMER_PARAM 3
#define CHASE_TIMER_PARAM 35
#define SCATTER_TIMER_PARAM 6
#define HURT_TIMER_PARAM 3
#define SPEED_PARAM 1
using std::vector;
using std::string;

class GameTile;

typedef vector<vector<vector<GameTile *>>> vec3pGT;
typedef vector<vector<GameTile *>> vec2pGT;
typedef vector<GameTile *> vec1pGT;

class Config {

public:
    ~Config();

    static Config *getInstance();

    Config(const Config &) = delete;

    Config &operator=(const Config &) = delete;

    sf::Vector2i map_size = {0, 0};
    const float tile_size = 22.f;
    // Написать в эту переменную абсолютный путь к директории проекта
    std::string path = "E:\\GitProjects\\LETI-Course-Project\\Pacman\\";
    sf::Vector2i window_size = {0, 0};
    const int window_framerate = 60;
    string window_title = "Pacman for kursach";
    string pause_window_title = "Pause";

    sf::Vector2f offset = {250.f, 75.f};
    sf::Image *icon;

    const float speed = SPEED_PARAM;

    const int frightened_timer = FRIGHTENED_TIMER_PARAM * window_framerate / tile_size;
    const int chase_timer = CHASE_TIMER_PARAM * window_framerate / tile_size;
    const int scatter_timer = SCATTER_TIMER_PARAM * window_framerate / tile_size;
    const int hurt_timer = HURT_TIMER_PARAM * window_framerate / tile_size;

    sf::Font *font{};

    std::map<const string, const string> maps = {
            {"single-player", path + "res\\maps\\single-player.map"},
            {"dual-player",   path + "res\\maps\\multi-player.map"},
            {"saved-player",  path + "res\\maps\\saved-player.map"},
    };
    string selected_map;
    const string pathToSave = path + "res\\maps\\saved-player.map";
    const string pathToSaveAndReadScore = path + "res\\maps\\points.txt";
    bool flagSave = false;
    bool loadFlag = false;
    bool mainMenuFlag = false;
    int score = 0;
    std::map<const string, sf::Sound *> sounds;
    std::map<const string, sf::Texture *> textures;

private:
    static Config *instance;

    Config();

    void loadSounds();

    void loadTextures();

    void loadFonts();

    void loadIcon();

    std::map<const string, sf::SoundBuffer *> buffers;

    std::map<const string, const string> sound_files = {
            {"background_music", path + "res\\audio\\background_music.wav"},
            {"background",       path + "res\\audio\\background.wav"},
            {"choice",           path + "res\\audio\\choice.wav"},
            {"start",            path + "res\\audio\\start.wav"},
            {"gg_vp",            path + "res/audio/gg_vp.wav"},
            {"exit",             path + "res/audio/exit.wav"},
            {"food",             path + "res/audio/food_chomp.wav"},
            {"fruit",            path + "res/audio/fruit_chomp.wav"},
            {"pellet",           path + "res/audio/power_pellet_chomp.wav"},
            {"pallets",          path + "res/audio/pallets.wav"},
            {"ghost",            path + "res/audio/ghost_chomp.wav"},
            {"pacman",           path + "res/audio/hurt.wav"},
            {"hurt",             path + "res/audio/have_hurt.wav"},
            {"press_start",      path + "res/audio/press_start.wav"},
            {"game_over",        path + "res/audio/game_over.wav"},
            {"thank_you",        path + "res/audio/thank_you.wav"},
            {"okey_dokey",       path + "res/audio/okey_dokey.wav"},
    };
    const std::string font_file = path + "res\\fonts/emulogic.ttf";
    std::map<const string, const string> texture_files = {
            {"empty",              path + "res\\sprites/empty.png"},
            {"iconImage",          path + "res\\sprites\\PacMan.png"},
            {"1",                  path + "res\\sprites/Walls/left-top.png"},
            {"2",                  path + "res\\sprites/Walls/horizontal.png"},
            {"3",                  path + "res\\sprites/Walls/right-top.png"},
            {"4",                  path + "res\\sprites/Walls/vertical.png"},
            {"5",                  path + "res\\sprites/Walls/left-bottom.png"},
            {"6",                  path + "res\\sprites/Walls/right-bottom.png"},
            {"!",                  path + "res\\sprites/Walls/double-top-left.png"},
            {"@",                  path + "res\\sprites/Walls/double-horizontal.png"},
            {"#",                  path + "res\\sprites/Walls/double-top-right.png"},
            {"$",                  path + "res\\sprites/Walls/double-vertical.png"},
            {"%",                  path + "res\\sprites/Walls/double-bottom-left.png"},
            {"^",                  path + "res\\sprites/Walls/double-bottom-right.png"},
            {"food",               path + "res\\sprites/Edibles/food.png"},
            {"power_pellet",       path + "res\\sprites/Edibles/power_pellet.png"},
            {"fruit_1",            path + "res\\sprites/Edibles/apple.png"},
            {"fruit_2",            path + "res\\sprites/Edibles/cherry.png"},
            {"fruit_3",            path + "res\\sprites/Edibles/lime.png"},
            {"fruit_4",            path + "res\\sprites/Edibles/peach.png"},
            {"fruit_5",            path + "res\\sprites/Edibles/pineapple.png"},
            {"fruit_6",            path + "res\\sprites/Edibles/strawberry.png"},
            {"Pacman_1_neutral",   path + "res\\sprites/Pacman1/neutral.png"},
            {"Pacman_1_left_1",    path + "res\\sprites/Pacman1/left_1.png"},
            {"Pacman_1_left_2",    path + "res\\sprites/Pacman1/left_2.png"},
            {"Pacman_1_right_1",   path + "res\\sprites/Pacman1/right_1.png"},
            {"Pacman_1_right_2",   path + "res\\sprites/Pacman1/right_2.png"},
            {"Pacman_1_up_1",      path + "res\\sprites/Pacman1/up_1.png"},
            {"Pacman_1_up_2",      path + "res\\sprites/Pacman1/up_2.png"},
            {"Pacman_1_down_1",    path + "res\\sprites/Pacman1/down_1.png"},
            {"Pacman_1_down_2",    path + "res\\sprites/Pacman1/down_2.png"},
            {"Pacman_2_neutral",   path + "res/sprites/Pacman2/neutral.png"},
            {"Pacman_2_left_1",    path + "res/sprites/Pacman2/left_1.png"},
            {"Pacman_2_left_2",    path + "res/sprites/Pacman2/left_2.png"},
            {"Pacman_2_right_1",   path + "res/sprites/Pacman2/right_1.png"},
            {"Pacman_2_right_2",   path + "res/sprites/Pacman2/right_2.png"},
            {"Pacman_2_up_1",      path + "res/sprites/Pacman2/up_1.png"},
            {"Pacman_2_up_2",      path + "res/sprites/Pacman2/up_2.png"},
            {"Pacman_2_down_1",    path + "res/sprites/Pacman2/down_1.png"},
            {"Pacman_2_down_2",    path + "res/sprites/Pacman2/down_2.png"},
            {"Blinky_left_1",      path + "res/sprites/Ghost/Blinky/left_1.png"},
            {"Blinky_left_2",      path + "res/sprites/Ghost/Blinky/left_2.png"},
            {"Blinky_right_1",     path + "res/sprites/Ghost/Blinky/right_1.png"},
            {"Blinky_right_2",     path + "res/sprites/Ghost/Blinky/right_2.png"},
            {"Blinky_up_1",        path + "res/sprites/Ghost/Blinky/up_1.png"},
            {"Blinky_up_2",        path + "res/sprites/Ghost/Blinky/up_2.png"},
            {"Blinky_down_1",      path + "res/sprites/Ghost/Blinky/down_1.png"},
            {"Blinky_down_2",      path + "res/sprites/Ghost/Blinky/down_2.png"},
            {"Clyde_left_1",       path + "res/sprites/Ghost/Clyde/left_1.png"},
            {"Clyde_left_2",       path + "res/sprites/Ghost/Clyde/left_2.png"},
            {"Clyde_right_1",      path + "res/sprites/Ghost/Clyde/right_1.png"},
            {"Clyde_right_2",      path + "res/sprites/Ghost/Clyde/right_2.png"},
            {"Clyde_up_1",         path + "res/sprites/Ghost/Clyde/up_1.png"},
            {"Clyde_up_2",         path + "res/sprites/Ghost/Clyde/up_2.png"},
            {"Clyde_down_1",       path + "res/sprites/Ghost/Clyde/down_1.png"},
            {"Clyde_down_2",       path + "res/sprites/Ghost/Clyde/down_2.png"},
            {"Inky_left_1",        path + "res/sprites/Ghost/Inky/left_1.png"},
            {"Inky_left_2",        path + "res/sprites/Ghost/Inky/left_2.png"},
            {"Inky_right_1",       path + "res/sprites/Ghost/Inky/right_1.png"},
            {"Inky_right_2",       path + "res/sprites/Ghost/Inky/right_2.png"},
            {"Inky_up_1",          path + "res/sprites/Ghost/Inky/up_1.png"},
            {"Inky_up_2",          path + "res/sprites/Ghost/Inky/up_2.png"},
            {"Inky_down_1",        path + "res\\sprites/Ghost/Inky/down_1.png"},
            {"Inky_down_2",        path + "res\\sprites/Ghost/Inky/down_2.png"},
            {"Pinky_left_1",       path + "res\\sprites/Ghost/Pinky/left_1.png"},
            {"Pinky_left_2",       path + "res\\sprites/Ghost/Pinky/left_2.png"},
            {"Pinky_right_1",      path + "res\\sprites/Ghost/Pinky/right_1.png"},
            {"Pinky_right_2",      path + "res\\sprites/Ghost/Pinky/right_2.png"},
            {"Pinky_up_1",         path + "res\\sprites/Ghost/Pinky/up_1.png"},
            {"Pinky_up_2",         path + "res\\sprites/Ghost/Pinky/up_2.png"},
            {"Pinky_down_1",       path + "res\\sprites/Ghost/Pinky/down_1.png"},
            {"Pinky_down_2",       path + "res\\sprites/Ghost/Pinky/down_2.png"},
            {"dead_left_1",        path + "res\\sprites/Ghost/dead/left_1.png"},
            {"dead_left_2",        path + "res\\sprites/Ghost/dead/left_2.png"},
            {"dead_right_1",       path + "res\\sprites/Ghost/dead/right_1.png"},
            {"dead_right_2",       path + "res\\sprites/Ghost/dead/right_2.png"},
            {"dead_up_1",          path + "res\\sprites/Ghost/dead/up_1.png"},
            {"dead_up_2",          path + "res\\sprites/Ghost/dead/up_2.png"},
            {"dead_down_1",        path + "res\\sprites/Ghost/dead/down_1.png"},
            {"dead_down_2",        path + "res\\sprites/Ghost/dead/down_2.png"},
            {"frightened_left_1",  path + "res\\sprites/Ghost/frightened/left_1.png"},
            {"frightened_left_2",  path + "res\\sprites/Ghost/frightened/left_2.png"},
            {"frightened_right_1", path + "res\\sprites/Ghost/frightened/right_1.png"},
            {"frightened_right_2", path + "res\\sprites/Ghost/frightened/right_2.png"},
            {"frightened_up_1",    path + "res\\sprites/Ghost/frightened/up_1.png"},
            {"frightened_up_2",    path + "res\\sprites/Ghost/frightened/up_2.png"},
            {"frightened_down_1",  path + "res\\sprites/Ghost/frightened/down_1.png"},
            {"frightened_down_2",  path + "res\\sprites/Ghost/frightened/down_2.png"},
    };

};

#endif