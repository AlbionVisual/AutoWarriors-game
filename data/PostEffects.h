#ifndef POST_EFFECTS_ONCE
#define POST_EFFECTS_ONCE

#include "../characters/Pawn.h"
#include <map>

/**
 * @brief Тип-функция для сохранения ссылки на какую-то из определённых функций-постэффектов
 *
 * @param self персонаж, который активирует постэффект
 * @param damager персонаж, который инициировал атаку на данного персонажа
 * @param extra_damage дополнитльный урон рассчитанный до удара со стороны нападающего (при
 *                     помощи эффектов)
 * @param move_number номер хода, на котором вызывается функция-эффект
 * @param external_printer функция для вывода информации о действии функции-эффекта
 *
 * @see Player::apply_damage(const Pawn&, int, int)
 * @see Enemy::apply_damage(const Pawn&, int, int)
 *
 */
using posteffect_func_type = std::function<int(const Pawn&, const Pawn&, int, int, printer_type)>;

/// @brief Теги для функций, в том числе `NO_POST_EFFECT`
enum PostEffects { SHIELD, STONE_SKIN, DOUBLE_DAMAGE_ON_CRUSHING, SLIMED_DAMAGE, NO_POST_EFFECT };

int shield(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer);

int stone_skin(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer);

int double_damage_on_crushing(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer);

int slimed_damage(const Pawn& self, const Pawn& damager, int extra_damage, int move_number, printer_type external_printer);

int no_posteffect(const Pawn& self, const Pawn& target, int extra_damage, int move_number, printer_type external_printer);

posteffect_func_type get_posteffect(PostEffects effect);

#endif // POST_EFFECTS_ONCE