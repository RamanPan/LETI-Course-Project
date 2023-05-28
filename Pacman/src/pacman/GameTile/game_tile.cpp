#include "game_tile.h"

GameTile::GameTile() = default;

GameTile::~GameTile() = default;

void GameTile::initVars() {
    frame_counter = 0;
}

void GameTile::initSprite() {
    sprite = sf::Sprite();
    loadTextures();
    sprite.setPosition(position);
}

void GameTile::loadTextures() {
    if (!textures.empty()) textures.clear();
    Config *config = Config::getInstance();
    for (std::string &name: texture_names) {
        textures.push_back(config->textures[name]);
    }
}

void GameTile::updateSprite() {
    const int animation_index = (frame_counter / animation_fps) % textures.size();
    sprite.setTexture(*(textures[animation_index]));
    frame_counter = (frame_counter + 1) % 60;
    sprite.setPosition({
                               position.x + Config::getInstance()->offset.x,
                               position.y + Config::getInstance()->offset.y,
                       });
}

void GameTile::render(sf::RenderTarget *_target) const {
    _target->draw(sprite);
}

GameTile &GameTile::operator=(const GameTile &_other) {
    position = _other.position;
    texture_names = _other.texture_names;
    textures = _other.textures;
    frame_counter = _other.frame_counter;
    sprite = _other.sprite;
    return *this;
}

