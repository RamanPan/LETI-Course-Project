#include "Game/game.h"
#include "MainMenu/main_menu.h"

int main(int argc, char **argv) {
    MainMenu().show();
    delete Config::getInstance();
    return 0;
}