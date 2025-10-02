
#include "Player.h"
#include "../data/PlayerData.h"

#include "../utils/ConsoleUtils.h"
#include "../data/PostEffects.h"
#include <algorithm> 
#include <iostream>

using namespace Players;
using std::vector;
using std::string;

/**
 * @brief Получает дополнительные данные, которые нужно именно добавить при атаке к характеристикам
 *
 * @param target На кого идёт удар этим персонажем
 * @param move_number Номер хода
 * @return pawn_specifications - Характеристики, которые нужно добавить перед ударом
 *
 * #### Example:
 *
 * Используется только в @see Pawn::attacked_by(Pawn&, int)
 *
 * Базовая функция в @see Pawn::get_extra_specifications(const Pawn&, int)
 * , а также есть упрощённый аналог в @see Enemy::get_extra_specifications(const Pawn&, int)
 */
pawn_specifications Player::get_extra_specifications(const Pawn& target, int move_number) {
    pawn_specifications extra_specifications = pawn_specifications(0, 0, 0);

    for (auto& pl_class : TYPES_VALUES) {
        int lvl = player_classes.at(pl_class);
        const vector<Effects>& all_effects = LEVEL_EFFECT_RELATION.at(pl_class);
        for (int i = 0; i < lvl; i++) {
            const Effects& effect = all_effects.at(i);
            // Если функция persistent
            if (is_persistent(effect)) {
                // то применить эффект жёстко только один раз
                if (!persitent_used[effect]) {
                    persitent_used[effect] = true;
                    specifications = specifications + get_effect(effect)(*this, target, move_number, external_printer);
                    continue;
                }
            }
            else {
                // иначе оставить информацию во временной структуре
                extra_specifications = extra_specifications + get_effect(effect)(*this, target, move_number, external_printer);
            }
        }
    }

    return extra_specifications;
};

/**
 * @brief Применение урона с учётом постэффектов для данных персонажей определённых в
 * @see Players::POSTEFFECT_RELATION
 *
 * @param damager Тот, кто наносит урон
 * @param extra_damage Посчитанное значение дополнительного урона (от эффектов)
 * @param move_number Номер хода
 *
 * #### Example:
 *
 * Используется только в:
 * @see Pawn::attacked_by(Pawn&, int)
 *
 */
void Player::apply_damage(const Pawn& damager, int extra_damage, int move_number) {
    int health_change = 0;
    /// @brief Нужно обновлять здоровье в любом случае, поэтому если ничего не применилось вызывается NO_POST_EFFECT в конце
    bool post_effect_applied = false;

    for (auto& pl_class : TYPES_VALUES) {
        int lvl = player_classes.at(pl_class);
        const vector<PostEffects>& all_effects = LEVEL_POSTEFFECT_RELATION.at(pl_class);
        for (int i = 0; i < lvl; i++) {
            const PostEffects& posteffect = all_effects.at(i);
            if (posteffect == NO_POST_EFFECT) continue;
            post_effect_applied = true;
            health_change += get_posteffect(posteffect)(*this, damager, extra_damage, move_number, external_printer);
        }
    }

    if (!post_effect_applied) health_change += get_posteffect(NO_POST_EFFECT)(*this, damager, extra_damage, move_number, external_printer);

    if (health_change > 0) health -= health_change;
    if (health < 0) health = 0;
}

/**
 * @brief Получает случайного игрока
 *
 * @return PlayerTypes - тег игрока
 */
PlayerTypes get_random_player_type() {
    return TYPES_VALUES[rand() % TYPES_AMOUNT];
}

/**
 * @brief Получение видимого имени при помощи @see Players::VISIBLE_NAMES
 *
 * @return string - Имя игрока, соответствующее сохранённой выборке мультикласса
 */
string Player::get_visible_name() {
    string res = "";
    for (auto& [player_type, amount] : player_classes) {
        if (amount != 0) res += VISIBLE_NAMES.at(player_type)[0] + std::to_string(amount);
    }
    return res;
};

/**
 * @brief Создаёт нового игрока с уже выбранным первым классом
 *
 * @param pl_class Выбранный класс
 *
 * #### Example:
 *
 * Единственный раз создаётся в @see main.cpp
 *
 * ```
 * Player pl = Player(OUTLAW);
 * GameDrawer game = GameDrawer(pl);
 * ```
 *
 */
Player::Player(PlayerTypes pl_class) {
    player_classes[pl_class] = 1;
    set_defaults();
    initial_health = health += specifications.endurance;
};

/**
 * @brief Создаёт игрока со случайным начальным классом
 *
 * #### Example:
 *
 * Нигде не используется
 *
 * ```
 * Player pl = Player();
 * GameDrawer game = GameDrawer(pl);
 * ```
 *
 */
Player::Player() {
    player_classes[get_random_player_type()] = 1;
    set_defaults();
    initial_health = health += specifications.endurance;
};

/**
 * @brief Устанавливает начальные данные для выбранного первого класса игрока. Использует
 *        при определении константы созданные в @see PlayerData.h
 *
 * #### Example:
 *
 * Используется только в конструкторах:
 * @see Player::Player()
 * и @see Player::Player(Players::PlayerTypes)
 *
 */
void Player::set_defaults() {
    PlayerTypes selected = NO_PLAYER_SELECTED;
    for (auto& [pl_class, amount] : player_classes) selected = pl_class;
    for (auto& it : TYPES_VALUES) {
        if (player_classes.count(it) == 0) player_classes[it] = 0;
    }

    if (selected != NO_PLAYER_SELECTED) {
        initial_health = health = HEALTHES.at(selected);
        active_weapon = DEFAULT_WEAPONS.at(selected);
    }
    else {};
}

/**
 * @brief Восстанавливает здоровье до значения до боя
 *
 * #### Example:
 *
 * Вызывается каждый раз после победы игрока
 * @see play_round(GameDrawer&, Player&, Enemy*, unsigned short&, bool)
 *
 */
void Player::reset_player() { health = initial_health; }

/**
 * @brief Добавляет "мультикласс" к игроку
 *
 * @param new_pl_class новый тег игрока для добавления способностей
 *
 * #### Example:
 *
 * Используется после победного боя, когда нужно поднять уровень игроку:
 * @see play_round(GameDrawer&, Player&, Enemy*, unsigned short&, bool)
 */
void Player::add_level(PlayerTypes new_pl_class) {
    player_classes[new_pl_class] += 1;
    initial_health = health += specifications.endurance;
};

/**
 * @brief Рассчитывает текущий уровень игрока как сумма всех уровней его подклассов
 *
 * @return int - текущий уровень игрока
 *
 * #### Example:
 *
 * Используется, когда нужно определить можно ли повышать ещё уровень:
 * @see play_round(GameDrawer&, Player&, Enemy*, unsigned short&, bool)
 */
int Player::get_level() {
    int sum = 0;
    for (auto& [pl_class, count] : player_classes) sum += count;
    return sum;
};