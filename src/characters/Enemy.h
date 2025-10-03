/**
 * @file Enemy.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Здесь описывается класс для доопределения врагов, а также перечисление их тегов
 * @date 2025-10-01
 *
 */

#ifndef ENEMY_ONCE
#define ENEMY_ONCE

#include "Pawn.h"
#include "../data/Effects.h"
#include <map>

using std::map;
using std::string;

namespace Enemies {
    /// @brief Теги для обозначения типа врагов
    enum Types { GOBLIN, SKELETON, SLIME, GHOST, GOLEM, DRAGON };

    Enemies::Types get_random_enemy_type();
}

/**
 * @brief Класс доопределяющий поведение врагов
 *
 */
class Enemy : public Pawn {
    /// @brief Тип врага
    Enemies::Types enemy_class;

    /// @brief Предмет, который будет выпадать из побеждённого врага
    Weapon reward;

    /// @brief Сохранение уже использованных единовременных эффектов
    map<Effects, bool> persitent_used;

    void set_defaults();
public:
    Enemy();

    pawn_specifications get_extra_specifications(const Pawn& target, int move_number) override;
    void apply_damage(const Pawn& damager, int extra_damage, int move_number) override;

    string get_visible_name();

    /**
     * @brief Получает поле с предопределённым оружием. Не соответствует активному оружию.
     *
     * @return Weapon Оружие, которое выпадает с врага, после победы над ним
     */
    Weapon get_reward() { return reward; };
};

#endif // ENEMY_ONCE