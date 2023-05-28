#ifndef GAME_TILE_HPP
#define GAME_TILE_HPP

#include "../Config/config.h"

class GameTile {

public:
    GameTile();

    virtual ~GameTile();

    virtual void render(sf::RenderTarget *) const;

    virtual void update(const sf::RenderTarget *, vec3pGT &) = 0;

    virtual bool isWalkable() const = 0;

    GameTile &operator=(const GameTile &);

    sf::Vector2f getPosition() const { return position; }

    virtual inline sf::Vector2i getMapPosition() const {
        const float tile_size = Config::getInstance()->tile_size;
        int x = static_cast<int>(position.x / tile_size);
        int y = static_cast<int>(position.y / tile_size);

        return {x, y};
    };

    virtual char whatIsThis() = 0;

public:

    sf::Sprite sprite;

    sf::Vector2f position;

    std::vector<std::string> texture_names;

    std::vector<sf::Texture *> textures;

    int frame_counter{};

    bool eat = false;

    static const int animation_fps = 8;

    virtual
    void initVars();

    void initSprite();

    void loadTextures();

    void updateSprite();

};

#endif