#include "Game/game.h"
#include "MainMenu/main_menu.h"

int main() {
    MainMenu().show();
    delete Config::getInstance();
    return 1;
}