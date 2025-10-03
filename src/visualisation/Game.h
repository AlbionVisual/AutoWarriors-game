/**
 * @file Game.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Здесь описывается класс контролирующий весь процесс игры
 * @date 2025-10-03
 *
 */

#ifndef GAME_ONCE
#define GAME_ONCE

#include "GameDrawer.h"

 /**
  * @brief Класс поставляющий методы для битв и выбора наград
  *
  */
class Game {

    /// @brief Место хранения игрока
    Player player;

    /// @brief Место хранения отрисовщика игры
    GameDrawer game;

    /// @brief Место хранения всех врагов
    Enemy enemy_list[5];

    /// @brief Количество созданных врагов
    unsigned short enemies_amount = 0;

    Players::PlayerTypes get_class_from_string(const string& player_class);
    Players::PlayerTypes ask_for_class();

    int play_fight();
    int play_round(bool is_last = false);

public:
    Game();
    void init();
    void wait(unsigned int seconds);
    void start();
};

#endif // GAME_ONCE