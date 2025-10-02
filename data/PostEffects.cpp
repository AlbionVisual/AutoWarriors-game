#include "PostEffects.h"

using std::to_string;
using std::map;

/**
 * @brief Функция, соответствующая постэффекту "щит". Если сила self больше силы damager, то
 *        урона будет нанесено на 3 меньше (как максимум)
 *
 * @param * Описаны в @see posteffect_func_type
 *
 * @return int - урон, наносимый персонажу за удар с учётом постэффекта
 *
 */
int shield(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer) {
    int actual_damage = damager.active_weapon.damage + extra_damage + damager.specifications.strength;
    if (self.specifications.strength > damager.specifications.strength) {
        if (external_printer) {
            external_printer("Shield worked (", 2, BLACK_WHITE);
            if (actual_damage <= 3) {
                external_printer("-" + to_string(actual_damage), 2, BLACK_RED);
                external_printer(" to damage; damage is absorbed)\n", 2, BLACK_WHITE);
            }
            else {
                external_printer(to_string(-3), 2, BLACK_RED);
                external_printer(" to damage)\n", 2, BLACK_WHITE);
            }
        }
        return actual_damage - 3;
    }
    return actual_damage;
}

/**
 * @brief Функция, соответствующая постэффекту "каменная кожа". Получаемы урон снижается на
 *        значение выносливости self
 *
 * @param * Описаны в @see posteffect_func_type
 *
 * @return int - урон, наносимый персонажу за удар с учётом постэффекта
 *
 */
int stone_skin(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer) {
    if (external_printer) {
        external_printer("Stone skin worked (", 2, BLACK_WHITE);
        external_printer("-" + to_string(self.specifications.endurance), 2, BLACK_RED);
        external_printer(" to damage)", 2, BLACK_WHITE);
    }
    return damager.active_weapon.damage + damager.specifications.strength + extra_damage - self.specifications.endurance;
}

/**
 * @brief Функция-постэффект, усиливающая действие дробящего оружия: увеличивается вдвое урон
 *
 * @param * Описаны в @see posteffect_func_type
 *
 * @return int - урон, наносимый персонажу за удар с учётом постэффекта
 *
 */
int double_damage_on_crushing(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer) {
    int actual_damage = damager.active_weapon.damage + extra_damage + damager.specifications.strength;
    if (damager.active_weapon.damage_type == CRUSHING) {
        if (external_printer) {
            external_printer("Used crashing (", 2, BLACK_WHITE);
            external_printer("+" + to_string(damager.active_weapon.damage), 2, BLACK_GREEN);
            external_printer(" to damage)", 2, BLACK_WHITE);
        }
        return actual_damage + damager.active_weapon.damage;
    }
    return actual_damage;
}

/**
 * @brief Функция-постэффект, ослабляющая рубящее оружее: убирает урон от оружия полностью, но
 *        оставляет от всего остального
 *
 * @param * Описаны в @see posteffect_func_type
 *
 * @return int - урон, наносимый персонажу за удар с учётом постэффекта
 *
 */
int slimed_damage(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer) {
    int actual_damage = damager.active_weapon.damage + extra_damage + damager.specifications.strength;
    if (damager.active_weapon.damage_type == CHOPPING) {
        if (external_printer) {
            external_printer("Slime absorption (", 2, BLACK_WHITE);
            external_printer("-" + to_string(damager.active_weapon.damage), 2, BLACK_RED);
            external_printer(" to damage)", 2, BLACK_WHITE);
        }
        return actual_damage - damager.active_weapon.damage;
    }
    return actual_damage;
}

/**
 * @brief Функция-постэффект, считающая урон без модификаций
 *
 * @param * Описаны в @see posteffect_func_type
 *
 * @return int - урон, наносимый персонажу за удар с учётом постэффекта
 *
 */
int no_posteffect(const Pawn& self, const Pawn& target, int extra_damage, int move_number, printer_type external_printer) {
    return target.active_weapon.damage + target.specifications.strength + extra_damage;
}

/// @brief Соотношение тегов постэффекта с его функцией
const map<PostEffects, posteffect_func_type> posteffect_enum_func_relation = {
    {SHIELD, shield}, {NO_POST_EFFECT, no_posteffect}, {STONE_SKIN, stone_skin},
    {SLIMED_DAMAGE, slimed_damage}, {DOUBLE_DAMAGE_ON_CRUSHING, double_damage_on_crushing}
};

/**
 * @brief Преобразовывает тег в вызываемую функцию
 *
 * @param effect тег, из которого нужно получить функцию
 * @return posteffect_func_type - вызываемая функция
 *
 * #### Example:
 *
 * ```
 * std::vector<Effects> effect_list {STONE_SKIN, NO_POST_EFFECT, SLUMED_DAMAGE};
 * get_posteffect(effect_list[rand() % 3]);
 * ```
 *
 * Смысл создания таких сложных структур, а также этой функции хорошо отображён в @see Player.cpp
 * и @see Enemy.cpp
 * в функциях @see Pawn::get_extra_specifications(const Pawn&, int)
 * , где ищется соответствие в константном словаре и при помощи этой функции получается
 * функция-постэффект
 *
 * @see Player::get_extra_specifications()
 * @see Enemy::get_extra_specifications()
 * @see effect_func_type
 */
posteffect_func_type get_posteffect(PostEffects effect) {
    return posteffect_enum_func_relation.at(effect);
};