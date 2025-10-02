#ifndef GAME_ONCE
#define GAME_ONCE

#include "GameDrawer.h"

class Game {

    Player player;
    GameDrawer game;
    Enemy enemy_list[5];
    unsigned short enemies_amount = 0;

    Players::PlayerTypes get_class_from_string(const string& player_class);
    Players::PlayerTypes ask_for_class();

    int play_fight(GameDrawer& game, Player& player, Enemy* enemy_list, unsigned short& amount);
    int play_round(GameDrawer& game, Player& player, Enemy* enemy_list, unsigned short& enemies_amount, bool is_last = false);

public:
    Game();
    void init();
    void wait(unsigned int seconds);
    void start();
};

#endif // GAME_ONCE