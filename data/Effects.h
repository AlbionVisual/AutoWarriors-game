/**
 * @file Effects.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Ряд функций-эффектов вида:
 *
 * ```
 * pawn_specifications effect_func(const Pawn& who_activated, const Pawn& target,
 * int move_number, printer_type external_printer)
 * ```
 *
 * @date 2025-10-01
 *
 */

#ifndef EFFECTS_ONCE
#define EFFECTS_ONCE

#include "../characters/Pawn.h"
#include <map>

 /**
  * @brief Тип-функция для сохранения ссылки на какую-то из определённых функций-эффектов
  *
  * @param self персонаж, который активирует эффект
  * @param target персонаж, на которого собирается действовать персонаж
  * @param move_number номер хода, на котором вызывается функция-эффект
  * @param external_printer функция для вывода информации о действии функции-эффекта
  *
  * @see Player::get_extra_specifications()
  * @see Enemy::get_extra_specifications()
  *
  */
using effect_func_type = std::function<pawn_specifications(const Pawn&, const Pawn&, int, printer_type)>;

/// @brief Теги для функций, в том числе `NO_EFFECT`
enum Effects { HIDDEN_ATTACK, RUSH_TO_ACTION, RAGE, STRENGTH_BOOST, AGILITY_BOOST, ENDURANCE_BOOTS, POISON, FIRE_BREATHING, NO_EFFECT };

pawn_specifications hidden_attack(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications rush_to_action(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications rage(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications strength_boost(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications agility_boost(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications endurance_boost(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications poison(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications fire_breathing(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

pawn_specifications no_effect(const Pawn& self, const Pawn& target, int move_number, printer_type external_printer);

effect_func_type get_effect(Effects effect);

bool is_persistent(Effects effect);

#endif // EFFECTS_ONCE