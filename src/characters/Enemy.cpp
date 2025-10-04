
#include "Enemy.h"
#include "../data/EnemyData.h"

using std::string;
using namespace Enemies;

/**
 * @brief Позволяет получить дополнительные характеристики, которые должны добавиться к
 *        текущим перед ударом. Учитывает все эффекты, которые накладываются на врага перед
 *        ударом (настраиваются в константе Enemies::EFFECT_RELATION) c учётом их частоты
 *        вызывания.
 *
 * @param target цель, на которую действует персонаж
 * @param move_number номер хода, для передачи в функции-эффекты
 * @param is_attacking Прошла ли атака (от этого зависит будет ли выводится информация)
 * @return pawn_specifications - Характеристики персонажа, которые нужно добавить к текущим
 *         перед ударом
 *
 * #### Example:
 *
 * Используется только в функции Pawn::attacked_by(Pawn&, int), также есть усложнённый аналог
 * для игрока Player::get_extra_specifications()
 *
 */
pawn_specifications Enemy::get_extra_specifications(const Pawn& target, int move_number, bool is_attacking) {
    pawn_specifications extra_specifications = pawn_specifications(0, 0, 0);
    const Effects& effect = EFFECT_RELATION.at(enemy_class);
    // Если функция persistent
    if (is_persistent(effect)) {
        // то применить эффект жёстко только один раз
        if (!persitent_used[effect]) {
            persitent_used[effect] = true;
            specifications = specifications + get_effect(effect)(*this, target, move_number, external_printer);
        }
    }
    else {
        // иначе оставить информацию во врЕменной структуре
        extra_specifications = extra_specifications + get_effect(effect)(*this, target, move_number, is_attacking ? external_printer : nullptr);
    }
    return extra_specifications;
};

/**
 * @brief Применение урона нанесённого другим персонажем с учётом собственных постэффектов
 *        определённых в константе Enemies::POSTEFFECT_RELATION
 *
 * @param damager Персонаж, который наносит урон
 * @param extra_damage Дополнительный урон, который считался в атакующем персонаже
 * @param move_number Номер хода, может быть нужен для некоторых постэффектов
 *
 * #### Example:
 *
 * Используется только в функции Pawn::attacked_by(Pawn&, int), также есть усложнённый аналог для игрока Player::apply_damage()
 *
 */
void Enemy::apply_damage(const Pawn& damager, int extra_damage, int move_number) {
    int health_change = 0;

    const PostEffects& posteffect = POSTEFFECT_RELATION.at(enemy_class);
    health_change += get_posteffect(posteffect)(*this, damager, extra_damage, move_number, external_printer);

    if (health_change > 0) health -= health_change;
    if (health < 0) health = 0;
}

/**
 * @brief Выбирает случайного персонажа
 *
 * @return Enemies::Types - тег выбранного персонажа
 *
 */
Types Enemies::get_random_enemy_type() {
    return TYPES_VALUES[rand() % TYPES_AMOUNT];
}

/**
 * @brief Создание нового экземпляра класса Enemy со случайным типом. Сразу устанавливаются
 * параметры при помощи метода Enemy::set_defaults()
 *
 * #### Example:
 *
 * ```
 * Enemy enemy1 = Enemy();
 * std::cout << enemy.get_visible_name();
 * ```
 *
 */
Enemy::Enemy() : enemy_class(get_random_enemy_type()) { set_defaults(); };

/**
 * @brief Получает видимое имя типа врага в виде строки
 *
 * @return string Имя типа врага
 *
 */
string Enemy::get_visible_name() {
    return VISIBLE_NAMES.at(enemy_class);
}

/**
 * @brief Устанавливает значения для полей (характеристики, начальное здоровье, награда, а
 *        также "активное оружие", с установленным значением урона, т.к. оно может не совпадать
 *        с наградным оружием) при помощи констант: Enemies::INITIAL_HEALTHES,
 *        Enemies::SPECIFICATIONS, Enemies::REWARDS, Enemies::WEAPON_DAMAGE
 *
 * #### Example:
 *
 * Используется только в конструкторе по умолчанию Enemy::Enemy()
 */
void Enemy::set_defaults() {
    health = INITIAL_HEALTHES.at(enemy_class) + specifications.endurance;
    specifications = SPECIFICATIONS.at(enemy_class);
    reward = REWARDS.at(enemy_class);
    active_weapon = Weapon(WEAPON_DAMAGE.at(enemy_class), reward.weapon_name);
}