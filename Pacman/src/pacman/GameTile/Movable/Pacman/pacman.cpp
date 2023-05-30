#include "pacman.h"
#include "../../../PauseMenu/pause_menu.h"

int Pacman::counter = 0;

Pacman::Pacman(const sf::Vector2f &_position) {
    initVars();
    index = 1;
    score = Config::getInstance()->score;
    position = _position;
    initText();
    initSprite();
}

Pacman::Pacman(const Pacman &_pacman) { *this = _pacman; }

Pacman &Pacman::operator=(const Pacman &_other) {
    this->Movable::operator=(_other);
    next_direction = _other.next_direction;
    score = _other.score;
    text = _other.text;

    return *this;
}

void Pacman::initVars() {
    Movable::initVars();
    counter++;
    index = counter;
    state = next_state = PacmanState::Chasing;
    resetTimer();
    texture_names = {"empty"};
    sound = Config::getInstance()->sounds["pacman"];
}

void Pacman::pollEvents() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        next_direction = Directions::Up;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        next_direction = Directions::Down;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        next_direction = Directions::Left;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        next_direction = Directions::Right;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        Config::getInstance()->score = score;
        PauseMenu().show();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5)) {
        Config::getInstance()->score = score;
        Config::getInstance()->flagSave = true;
    }
}

void Pacman::initText() {
    Config *config = Config::getInstance();
    text.setFont(*(config->font));
    text.setCharacterSize(20.f);
    text.setFillColor(sf::Color::Yellow);
    text.setStyle(sf::Text::Regular);
    text.setString(
            "Player " + std::to_string(1) + "\nScore: " + std::to_string(score)
    );
    text.setPosition({
                             5,
                             config->offset.y + config->tile_size,
                     });
}

void Pacman::interact(vec3pGT &_map) {
    if (state == PacmanState::Hurt) {
        return;
    }
    sf::Vector2i map_pos = getMapPosition();
    vec1pGT &vector = _map[map_pos.y][map_pos.x];
    for (GameTile *tile: vector) {
        if (dynamic_cast<PowerPellet *>(tile) != nullptr) {
            auto *pellet = (PowerPellet *) tile;
            if (!pellet->isEaten()) {
                for (vec2pGT &vec2: _map) {
                    for (vec1pGT &vec1: vec2) {
                        for (GameTile *gt: vec1) {
                            if (dynamic_cast<Ghost *>(gt) != nullptr) {
                                ((Ghost *) gt)->toFrightenedState();
                            }
                        }
                    }
                }
                pellet->toEatenState();
                return;
            }
        }
        if (dynamic_cast<Edible *>(tile) != nullptr) {
            auto *edible = (Edible *) tile;
            if (!edible->isEaten()) {
                score += edible->getScoreModifier();
                updateScore();
                edible->toEatenState();
                return;
            }
        }

        if (dynamic_cast<Ghost *>(tile) != nullptr) {
            auto *ghost = dynamic_cast<Ghost *>(tile);
            if (ghost->isScared() && !ghost->isDead()) {
                score += 200;
                updateScore();
                ghost->toDeadState();
                return;
            }

            if (!ghost->isScared() && !ghost->isDead()) {
                toDeadState();
                return;
            }
        }
    }
}

void Pacman::updateState() {
    if (state != next_state) {
        switch (next_state) {
            case PacmanState::Hurt:
                sound->play();
                state = PacmanState::Hurt;
                score -= 200;
                updateScore();
                break;
            case PacmanState::Chasing:
                break;
        }
        updateAnimation();
        return;
    }
    if (state == PacmanState::Hurt) {
        if (hurt_timer > 0) return;
        state = PacmanState::Chasing;
        next_state = state;
        resetTimer();
        updateAnimation();
        return;
    }
}

void Pacman::updateMovementDirection(vec3pGT &_map) {
    const sf::Vector2i map_position = getMapPosition();
    const sf::Vector2f closest_position = {
            map_position.x * Config::getInstance()->tile_size,
            map_position.y * Config::getInstance()->tile_size,
    };
    if (position != closest_position) {
        return;
    }
    updateState();
    const sf::Vector2i tile_position = map_position + next_direction;
    vec1pGT &vector = _map[tile_position.y][tile_position.x];
    for (GameTile *tile: vector) {
        if (tile != nullptr && !tile->isWalkable()) {
            return;
        }
    }
    direction = next_direction;
    updateTimer();
    updateAnimation();
}

void Pacman::updateAnimation() {
    std::string direction_name;
    switch (direction.x + direction.y * 10) {
        case -10:
            direction_name = "up";
            break;
        case -1:
            direction_name = "left";
            break;
        case 10:
            direction_name = "down";
            break;
        default:
        case 1:
            direction_name = "right";
            break;
    }

    if (state == PacmanState::Chasing) {
        texture_names = {
                "Pacman_" + std::to_string(index) + "_neutral",
                "Pacman_" + std::to_string(index) + "_" + direction_name + "_2",
                "Pacman_" + std::to_string(index) + "_" + direction_name + "_1",
                "Pacman_" + std::to_string(index) + "_" + direction_name + "_2"
        };
    } else {
        texture_names = {
                "Pacman_" + std::to_string(index) + "_neutral",
                "empty",
                "Pacman_" + std::to_string(index) + "_" + direction_name + "_2",
                "empty",
                "Pacman_" + std::to_string(index) + "_" + direction_name + "_1",
                "empty",
                "Pacman_" + std::to_string(index) + "_" + direction_name + "_2"
                                                                           "empty",
        };

    }
    loadTextures();
}

void Pacman::update(const sf::RenderTarget *_target, vec3pGT &_map) {
    pollEvents();
    sf::Vector2i map_pos = getMapPosition();
    vec1pGT &vector = _map[map_pos.y][map_pos.x];
    if (!vector.empty())
        vector.erase(std::remove(vector.begin(), vector.end(), this), vector.end());
    updateMovementDirection(_map);
    position = {
            position.x + direction.x * speed,
            position.y + direction.y * speed
    };
    checkCollisions(_target, _map);
    interact(_map);
    map_pos = getMapPosition();
    _map[map_pos.y][map_pos.x].push_back(this);
    updateSprite();
}

void Pacman::render(sf::RenderTarget *_target) const {
    _target->draw(sprite);
    _target->draw(text);
}

void Pacman::checkWindowCollisions(const sf::RenderTarget *_target) {
    const sf::Vector2u window_size = _target->getSize();
    const sf::Vector2f size = {
            sprite.getGlobalBounds().width,
            sprite.getGlobalBounds().height,
    };
    if (position.x < 0.f) {
        position.x = 0.f;
    } else if (position.x + size.x > window_size.x) {
        position.x = window_size.x - size.x;
    }
    if (position.y < 0.f) {
        position.y = 0.f;
    } else if (position.y + size.y > window_size.y) {
        position.y = window_size.y - size.y;
    }
}

void Pacman::collideWithObjects(vec3pGT &_map) {
    const sf::Vector2i delta = {
            (direction.x > 0) ? 1 : 0,
            (direction.y > 0) ? 1 : 0
    };
    const sf::Vector2i current_position = getMapPosition() + delta;
    vec1pGT &vector = _map[current_position.y][current_position.x];
    for (GameTile *tile: vector) {
        if (tile != nullptr) {
            if (!tile->isWalkable() || dynamic_cast<Pacman *>(tile) != nullptr) {
                const int tile_size = Config::getInstance()->tile_size;
                position = {
                        static_cast<float>(round(position.x / tile_size) * tile_size),
                        static_cast<float>(round(position.y / tile_size) * tile_size),
                };
                return;
            }
        }
    }
}

void Pacman::checkCollisions(const sf::RenderTarget *_target, vec3pGT &_map) {
    checkWindowCollisions(_target);
    collideWithObjects(_map);
}

void Pacman::toDeadState() { next_state = PacmanState::Hurt; }

void Pacman::updateScore() {
    text.setString(
            "Player " + std::to_string(1) + "\nScore: " + std::to_string(score)
    );
}
