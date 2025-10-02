/**
 * @file GameDrawer.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Помощник в отрисовывании игры
 * @date 2025-10-01
 *
 */

#ifndef GAME_DRAWER_ONCE
#define GAME_DRAWER_ONCE

#include "../characters/Player.h"
#include "../characters/Enemy.h"
#include "../utils/ConsoleUtils.h"
#include <vector>
#include <iostream>

using namespace std;


/**
 * @brief Вспомогательная структура для передачи сообщения с учётом времени отображения и
 *        цвета
 *
 */
struct Message {
    Message() {};
    Message(string text, int seconds, ConsoleColor color = BLACK_WHITE) : text(text), color(color), seconds(seconds) {};

    /// @brief Текст сообщения
    string text;

    /// @brief Цвет сообщения, соответствующий возможным цветам консоли
    ConsoleColor color;

    /// @brief Время в секундах отображения на экране (без учёта ввода игрока)
    int seconds;
};

/**
 * @brief Класс-помощник в отображении информации о статусе игры. Использует Кэш для
 *        определения: какие значения поменялись и автоматически подсвечивает эти значения
 *        красным или зелёным в зависимости от положительности изменения числа. При обновлении
 *        полностью стирает консоль
 *        @see clear_console()
 *        и перерисовывает всё заново.
 *
 */
class GameDrawer {
    /// @brief Текущее состояние игрока
    Player* player_state;

    /// @brief Текущее состояние врага, с которым идёт бой
    Enemy* active_enemy_state;

    /// @brief Количество врагов на поле (и побеждённые, и активный учитываются)
    size_t enemies_amount = 0;

    /// @brief Сохраняет направление стрелки соответствующее направлению атаки (слева игрок, справа его враг)
    bool attack_left = false;

    /// @brief Список всех сообщений для отображения (удаляются при seconds = 0)
    vector<Message> message_list;

    /// @brief Сохранённое значение персонажа игрока с прошлого раунда для сравнения с новым
    Player cashed_player_state;

    /// @brief Сохранённые значения персонажей врагов с прошлых раундов для сравнения с новым, а также для вывода истории
    Enemy cashed_enemies_state[5];

    bool colored_print(int n1, int n2);
    void sync_cache();
public:
    GameDrawer() {};
    GameDrawer(Player& pl);

    void freeze(size_t seconds = 1);

    void add_enemy(Enemy& en);

    void change_attack_direction(bool new_direction);

    void redraw_console();

    void add_message(string text, int seconds, ConsoleColor color = BLACK_WHITE);

};

#endif // GAME_DRAWER_ONCE