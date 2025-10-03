#include "Effects.h"

using std::map;

/**
 * @brief Функция-эффект соответствующая навыку "Скрытая атака" effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications hidden_attack(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    if (self.specifications.agility > target.specifications.agility) {
        if (external_printer) {
            external_printer("Hidden attack activated! (", 2, BLACK_WHITE);
            external_printer("+1", 2, BLACK_GREEN);
            external_printer(" to damage)\n", 2, BLACK_WHITE);
        }
        return pawn_specifications(1, 0, 0);
    }
    return pawn_specifications(0, 0, 0);
}

/**
 * @brief Функция-эффект соответствующая навыку "Порыв к действию" effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications rush_to_action(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    if (move_number <= 1) {
        if (external_printer) {
            external_printer("Rush to action used! (", 2, BLACK_WHITE);
            external_printer("+" + std::to_string(self.active_weapon.damage), 2, BLACK_GREEN);
            external_printer(" to damage)\n", 2, BLACK_WHITE);
        }
        return pawn_specifications(self.active_weapon.damage, 0, 0);
    }
    return pawn_specifications(0, 0, 0);
}

/**
 * @brief Функция-эффект соответствующая навыку "Ярость" effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications rage(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    if (move_number <= 6) {
        if (external_printer) {
            external_printer("Rage (", 2, BLACK_WHITE);
            external_printer("+2", 2, BLACK_GREEN);
            external_printer(" to damage)\n", 2, BLACK_WHITE);
        }
        return pawn_specifications(2, 0, 0);
    }
    if (external_printer) {
        external_printer("Rage (", 2, BLACK_WHITE);
        external_printer("-1", 2, BLACK_RED);
        external_printer(" to damage)\n", 2, BLACK_WHITE);
    }
    return pawn_specifications(-1, 0, 0);
}

/**
 * @brief Функция-эффект увеличивающая силу на единицу каждый бой effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications strength_boost(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    if (move_number <= 1) return pawn_specifications(1, 0, 0);
    return pawn_specifications(0, 0, 0);
}

/**
 * @brief Функция-эффект увеличивающая ловкость на единицу каждый бой effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications agility_boost(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    if (move_number <= 1) return pawn_specifications(0, 1, 0);
    return pawn_specifications(0, 0, 0);
}

/**
 * @brief Функция-эффект увеличивающая выносливость на единицу каждый бой effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications endurance_boost(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    return pawn_specifications(0, 0, 1);
}

/**
 * @brief Функция-эффект соответствующая способности "яд" effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications poison(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    if (external_printer) {
        external_printer("POISON (", 2, BLACK_WHITE);
        external_printer("+" + std::to_string(move_number / 2 + 1), 2, BLACK_GREEN);
        external_printer(" to damage)\n", 2, BLACK_WHITE);
    }
    return pawn_specifications(move_number / 2 + 1, 0, 0);
}

/**
 * @brief Функция-эффект соответствующая навыку "Дыхание огнём" effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications - Изменения в характеристиках, но не сами характеристики!
 *
 */
pawn_specifications fire_breathing(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    if (move_number % 6 >= 4) {
        if (external_printer) {
            external_printer("Fire breath! (", 2, BLACK_WHITE);
            external_printer("+" + std::to_string(3), 2, BLACK_GREEN);
            external_printer(" to damage)\n", 2, BLACK_WHITE);
        }
        return pawn_specifications(3, 0, 0);
    }
    return pawn_specifications(0, 0, 0);
}

/**
 * @brief Пустая функция-эффект для значений по умолчанию effect_func_type
 *
 * @param * Описаны в effect_func_type как общие для группы функций
 *
 * @return pawn_specifications(0, 0, 0)
 *
 */
pawn_specifications no_effect(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer) {
    return pawn_specifications(0, 0, 0);
}

/// @brief Вспомогательный константный словарь для быстрого поиска функции по тегу
const map<Effects, effect_func_type> effect_enum_func_relation = {
    {HIDDEN_ATTACK, hidden_attack},
    {RUSH_TO_ACTION, rush_to_action},
    {RAGE, rage},
    {STRENGTH_BOOST, strength_boost},
    {AGILITY_BOOST, agility_boost},
    {ENDURANCE_BOOTS, endurance_boost},
    {POISON, poison},
    {FIRE_BREATHING, fire_breathing},
    {NO_EFFECT, no_effect}
};
/**
 * @brief Преобразовывает тег в вызываемую функцию
 *
 * @param effect тег, из которого нужно получить функцию
 * @return effect_func_type - вызываемая функция
 *
 * #### Example:
 *
 * ```
 * std::vector<Effects> effect_list {FIRE_BREATHING, NO_EFFECT, ENDURANCE_BOOST};
 * get_effect(effect_list[rand() % 3]);
 * ```
 *
 * Смысл создания таких сложных структур, а также этой функции хорошо отображён в Player.cpp
 * и Enemy.cpp в функциях get_extra_specifications, где ищется соответствие в константном
 * словаре и при помощи этой функции получается функция
 *
 * @see Player::get_extra_specifications(const Pawn&, int)
 * @see Enemy::get_extra_specifications(const Pawn&, int)
 * @see effect_func_type
 */
effect_func_type get_effect(Effects effect) {
    return effect_enum_func_relation.at(effect);
};

/// @brief Вспомогательный константный словарь для быстрой настройки типа функций
const map<Effects, bool> is_func_persistent = {
    {HIDDEN_ATTACK, false},
    {RUSH_TO_ACTION, false},
    {RAGE, false},
    {STRENGTH_BOOST, true},
    {AGILITY_BOOST, true},
    {ENDURANCE_BOOTS, true},
    {POISON, false},
    {FIRE_BREATHING, false},
    {NO_EFFECT, true}
};
/**
 * @brief Определяет тип функции: перманентная или динамическая. Нужно для того, чтобы
 *        определить как часто применять эффект (один раз или каждый удар)
 *
 * @param effect Тег, указывающий на тестируемую функцию
 * @return true Результат функции должен применяться реже
 * @return false Результат применяется каждый удар
 *
 * #### Example:
 *
 * Смысл создания таких сложных структур, а также этой функции хорошо отображён в `./Player.cpp`
 * и Enemy.cpp в функциях get_extra_specifications, где ищется соответствие в константном
 * словаре
 *
 * @see Player::get_extra_specifications(const Pawn&, int)
 * @see Enemy::get_extra_specifications(const Pawn&, int)
 * @see effect_func_type
 */
bool is_persistent(Effects effect) {
    return is_func_persistent.at(effect);
};