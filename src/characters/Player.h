/**
 * @file Player.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Здесь описывается класс, доопределяющий поведение персонажа для игрока, а также
 *        перечисление тегов игроков
 * @date 2025-10-03
 *
 */

#ifndef PLAYER_ONCE
#define PLAYER_ONCE

#include "Pawn.h"
#include <functional>
#include <map>
#include "../data/Effects.h"

namespace Players {
    /// @brief Теги для игроков, а также заглушка NO_PLAYER_SELECTED
    enum PlayerTypes { NO_PLAYER_SELECTED, OUTLAW, WARRIOR, BARBARIAN };
}

/**
 * @brief Класс доопределяющий логику для персонажей игрока
 *
 */
class Player : public Pawn {

    /// @brief Текущее состояние мультикласса игрока (содержит все классы и количества их выбора пользователем)
    std::map<Players::PlayerTypes, int> player_classes;

    /// @brief Начальное здоровье сохраняется для восполнения к этому значению после каждого боя
    int initial_health;

    /// @brief Использованные эффекты (есть проверка этого массива на повторяемость для единовременных эффектов)
    std::map<Effects, bool> persitent_used;

    void set_defaults();
public:
    Player();
    Player(Players::PlayerTypes pl_class);
    std::string get_visible_name();
    pawn_specifications get_extra_specifications(const Pawn& target, int move_number, bool is_attacking) override;
    void apply_damage(const Pawn& damager, int extra_damage, int move_number) override;
    void reset_player();
    void add_level(Players::PlayerTypes new_pl_class);
    int get_level();

};

#endif // PLAYER_ONCE