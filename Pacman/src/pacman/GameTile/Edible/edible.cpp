 #include "edible.h"

Edible::Edible(const std::string &buffer_name) {
    score_modifier = 0;
    state = EdibleState::Active;

    sound = Config::getInstance()->sounds[buffer_name];
}
Edible::Edible(const Edible &_edible) { *this = _edible; }
Edible::~Edible() = default;

Edible &Edible::operator=(const Edible &_other) {
    this->GameTile::operator=(_other);
    score_modifier = _other.score_modifier;
    sound = _other.sound;

    return *this;
}

void Edible::update(const sf::RenderTarget *, vec3pGT &_map) {
    updateSprite();
}

void Edible::toEatenState() {
    score_modifier = 0;
    texture_names = { "empty" };
    state = EdibleState::Eaten;
    eat = true;
    sound->play();
    loadTextures();
}
