#include "ghost.h"

Ghost::Ghost(const std::string &_name, const sf::Vector2f &_position) {
    initVars();

    position = _position;
    name = _name;
    resetTimers();

    initSprite();
    initTrail();
}

Ghost::Ghost(const Ghost &_ghost) { *this = _ghost; }

Ghost &Ghost::operator=(const Ghost &_other) {
    this->Movable::operator=(_other);
    chasing = _other.chasing;
    state = _other.state;
    frightened_timer = _other.frightened_timer;
    name = _other.name;
    rng = _other.rng;

    return *this;
}

void Ghost::initVars() {
    Movable::initVars();
    chasing = {};
    rng = RNG();
    state = next_state = GhostStates::Escape;
    resetTimers();
    texture_names = {"empty"};
    sound = Config::getInstance()->sounds["ghost"];
}

void Ghost::initTrail() {
    trail.setSize({
                          Config::getInstance()->tile_size,
                          Config::getInstance()->tile_size
                  });
    trail.setFillColor(sf::Color::Transparent);
    trail.setOutlineThickness(2.f);
    sf::Color trail_color;
    switch (name.at(0)) {
        case 'B': // Blinky
            trail_color = sf::Color::Red;
            break;
        case 'P': // Pinky
            trail_color = sf::Color(255, 192, 203); // PINK
            break;
        case 'I': // Inky
            trail_color = sf::Color::Blue;
            break;
        case 'C': // Clyde
            trail_color = sf::Color(255, 165, 0); // ORANGE
            break;
    }
    trail.setOutlineColor(trail_color);
}

void Ghost::updateAnimation() {
    std::string current_direction;
    switch (direction.x + direction.y * 10) {
        case -10:
            current_direction = "up";
            break;
        case -1:
            current_direction = "left";
            break;
        case 10:
            current_direction = "down";
            break;
        case 1:
        default:
            current_direction = "right";
            break;
    }
    std::string prefix;
    switch (state) {
        case GhostStates::Frightened:
            prefix = "frightened";
            break;
        case GhostStates::Dead:
            prefix = "dead";
            break;
        default:
            prefix = name;
            break;
    }
    texture_names = {
            prefix + "_" + current_direction + "_1",
            prefix + "_" + current_direction + "_2"
    };
    loadTextures();
}

bool Ghost::isInsideGhostHouse() {
    const sf::Vector2i map_position = getMapPosition();
    return (map_position.x <= 16 && map_position.x >= 11 && map_position.y <= 15 && map_position.y >= 13);
}

bool Ghost::isInFrontOfGhostHouse() {
    const sf::Vector2i map_position = getMapPosition();
    return (map_position.x > 12 && map_position.x <= 14 && map_position.y == 11);
}

void Ghost::updateTrail() {
    ;
    Config *config = Config::getInstance();
    trail.setPosition({
                              trail_position.x * config->tile_size + config->offset.x,
                              trail_position.y * config->tile_size + config->offset.y,
                      });
}

void Ghost::resetTimers() {
    frightened_timer = Config::getInstance()->frightened_timer;
    chase_timer = Config::getInstance()->chase_timer;
    scatter_timer = Config::getInstance()->scatter_timer;

}

void Ghost::updateTimers() {
    if (state == GhostStates::Scatter) scatter_timer--;
    if (state == GhostStates::Chase) chase_timer--;
    if (state == GhostStates::Frightened) frightened_timer--;
}

void Ghost::updateMovementDirection(vec3pGT &_map) {
    const sf::Vector2i map_position = getMapPosition();
    const sf::Vector2f closest_position = {
            map_position.x * Config::getInstance()->tile_size,
            map_position.y * Config::getInstance()->tile_size,
    };
    if (position != closest_position) {
        return;
    }
    updateState();
    sf::Vector2i next_direction = {};
    switch (state) {
        case GhostStates::Frightened:
            next_direction = frightened(_map);
            trail_position = getMapPosition() + direction;
            break;
        case GhostStates::Dead:
            next_direction = chase(_map, home_position);
            trail_position = home_position;
            break;
        case GhostStates::Scatter:
            next_direction = chase(_map, scatter_position);
            trail_position = scatter_position;
            break;
        case GhostStates::Chase:
            next_direction = chase(_map, getChasePosition());
            trail_position = getChasePosition();
            break;
        case GhostStates::Escape:
            next_direction = chase(_map, escape_position);
            trail_position = escape_position;
            break;
    }
    if (isInFrontOfGhostHouse() && state != GhostStates::Dead && next_direction == Directions::Down) {
        next_direction = direction;
    }
    direction = next_direction;
    updateTimers();
}

void Ghost::render(sf::RenderTarget *_target) const {
    _target->draw(sprite);
}

void Ghost::updateState() {
    ;
    if (state != next_state) {
        switch (next_state) {
            case GhostStates::Dead:
                if (state != GhostStates::Frightened) {
                    next_state = state;
                    break;
                }
                sound->play();
                state = next_state;
                speed = Config::getInstance()->speed * 2;
                resetTimers();
                loadTextures();
                break;
            case GhostStates::Frightened:
                if (state == GhostStates::Dead) {
                    next_state = state;
                    break;
                }
                state = next_state;
                speed = 0.5 * Config::getInstance()->speed;
                resetTimers();
                loadTextures();
                break;
            default:
                break;
        }
    }
    switch (state) {
        case GhostStates::Escape:
            if (getMapPosition() != escape_position) break;
            state = next_state = GhostStates::Scatter;
            speed = Config::getInstance()->speed;
            resetTimers();
            loadTextures();
            break;
        case GhostStates::Dead:
            if (getMapPosition() != home_position) break;
            state = next_state = GhostStates::Escape;
            speed = Config::getInstance()->speed;
            resetTimers();
            loadTextures();
            break;
        case GhostStates::Chase:
            if (chase_timer > 0) break;
            state = next_state = GhostStates::Scatter;
            speed = Config::getInstance()->speed;
            resetTimers();
            loadTextures();
            break;
        case GhostStates::Scatter:
            if (scatter_timer > 0) break;
            state = next_state = GhostStates::Chase;
            speed = Config::getInstance()->speed;
            resetTimers();
            loadTextures();
            break;
        case GhostStates::Frightened:
            if (frightened_timer > 0) break;
            state = next_state = GhostStates::Chase;
            speed = Config::getInstance()->speed;
            resetTimers();
            loadTextures();
            break;
    }
}

bool Ghost::canMove(vec3pGT &_map, const sf::Vector2i &_dir) const {
    if (direction == -_dir) {
        return false;
    }
    sf::Vector2i new_position = getMapPosition() + _dir;
    vec1pGT &vector = _map[new_position.y][new_position.x];
    for (GameTile *tile: vector) {
        if (tile != nullptr && !tile->isWalkable()) {
            return false;
        }
    }
    return true;
}

sf::Vector2i Ghost::frightened(vec3pGT &_map) {
    std::vector<sf::Vector2i> available_directions = {};
    if (canMove(_map, Directions::Up)) available_directions.push_back(Directions::Up);
    if (canMove(_map, Directions::Down)) available_directions.push_back(Directions::Down);
    if (canMove(_map, Directions::Left)) available_directions.push_back(Directions::Left);
    if (canMove(_map, Directions::Right)) available_directions.push_back(Directions::Right);
    if (available_directions.empty()) {
        return Directions::None;
    }
    return available_directions.at(rng.get(0, available_directions.size() - 1));
}

int Ghost::rankMove(vec3pGT &_map, const sf::Vector2i &_dest, const sf::Vector2i &_dir) const {
    if (direction == -_dir) {
        return NUMBER_BIGGER;
    }
    sf::Vector2i new_position = getMapPosition() + _dir;
    vec1pGT &vector = _map[new_position.y][new_position.x];
    for (GameTile *tile: vector) {
        if (tile != nullptr && !tile->isWalkable()) {
            return NUMBER_BIGGER;
        }
    }
    const int deltaX = _dest.x - new_position.x;
    const int deltaY = _dest.y - new_position.y;
    return (deltaX * deltaX) + (deltaY * deltaY);
}

sf::Vector2i Ghost::chase(vec3pGT &_map, const sf::Vector2i &_destination) const {
    ;
    const int up = rankMove(_map, _destination, Directions::Up);
    const int left = rankMove(_map, _destination, Directions::Left);
    const int down = rankMove(_map, _destination, Directions::Down);
    const int right = rankMove(_map, _destination, Directions::Right);
    int reference = NUMBER_BIGGER;
    sf::Vector2i best_direction = Directions::None;
    if (up < reference) {
        reference = up;
        best_direction = Directions::Up;
    }
    if (left < reference) {
        reference = left;
        best_direction = Directions::Left;
    }
    if (down < reference) {
        reference = down;
        best_direction = Directions::Down;
    }
    if (right < reference) {
        reference = right;
        best_direction = Directions::Right;
    }
    return best_direction;
}

void Ghost::update(const sf::RenderTarget *_target, vec3pGT &_map) {
    sf::Vector2i map_pos = getMapPosition();
    vec1pGT &vector = _map[map_pos.y][map_pos.x];
    vector.erase(std::remove(vector.begin(), vector.end(), this));
    updateMovementDirection(_map);
    position = {
            position.x + direction.x * speed,
            position.y + direction.y * speed
    };
    map_pos = getMapPosition();
    _map[map_pos.y][map_pos.x].push_back(this);
    updateAnimation();
    updateSprite();
    updateTrail();
}