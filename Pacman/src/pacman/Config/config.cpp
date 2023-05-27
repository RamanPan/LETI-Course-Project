#include "config.h"

Config *Config::instance = nullptr;

Config::Config() {
    loadSounds();
    loadTextures();
    loadFonts();
    loadIcon();
}

Config::~Config() {
    for (auto &entry: sounds) delete entry.second;
    for (auto &entry: buffers) delete entry.second;
    for (auto &entry: textures) delete entry.second;
    delete font;
}

Config *Config::getInstance() {
    if (instance == nullptr) instance = new Config();
    return instance;
}

void Config::loadSounds() {
    for (auto &file: sound_files) {
        auto *buffer = new sf::SoundBuffer();
        if (!buffer->loadFromFile(file.second)) {
            std::cout << "ERROR: Failed to load " << file.second << std::endl;
            abort();
        }
        auto *sound = new sf::Sound(*buffer);

        buffers.emplace(file.first, buffer);
        sounds.emplace(file.first, sound);
    }
}

void Config::loadTextures() {
    for (auto &file: texture_files) {
        auto *texture = new sf::Texture();

        if (!texture->loadFromFile(file.second)) {
            std::cout << "ERROR: Failed to load " << file.second << std::endl;
            abort();
        }

        textures.emplace(file.first, texture);
    }
}

void Config::loadFonts() {
    font = new sf::Font();

    if (!font->loadFromFile(font_file)) {
        std::cout << "ERROR: Failed to loead " << font_file << std::endl;
        abort();
    }
}

void Config::loadIcon() {
    icon = new sf::Image();
    if (!icon->loadFromFile(texture_files.at("iconImage"))) {
        std::cout << "ERROR: Failed to load icon image" << std::endl;
        abort();
    }
}

