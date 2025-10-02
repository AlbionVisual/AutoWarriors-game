#ifndef PLAYERDATA_ONCE
#define PLAYERDATA_ONCE

#include "../characters/Player.h"
#include "PostEffects.h"

using std::map;
using std::string;
using std::vector;

namespace Players {

    /// @brief Теги игроков в массиве для доступа к ним по индексу
    constexpr PlayerTypes TYPES_VALUES[] = { OUTLAW, WARRIOR, BARBARIAN };

    /// @brief Количество игроков (размер массива выше)
    const short TYPES_AMOUNT = 3;

    /// @brief Соотношение тега игрока с начальным значением его здоровья
    const map<PlayerTypes, int> HEALTHES = { {OUTLAW, 4}, {WARRIOR, 5}, {BARBARIAN, 6} };

    /// @brief Соотношение тегов игроков с их начальными оружиями
    const map<PlayerTypes, const Weapon>DEFAULT_WEAPONS = {
        {OUTLAW, DAGGER}, {WARRIOR, SWORD}, {BARBARIAN, CLUB}
    };

    /// @brief Соотношение тегов игроков с их строковыми именами
    const map<PlayerTypes, string>VISIBLE_NAMES = {
        {OUTLAW, "Outlaw"}, {WARRIOR, "Warrior"}, {BARBARIAN, "Barbarian"}
    };

    /// @brief Соотношение тегов игроков с соответствующими бонусами (эффектами) на каждом из трёх уровней
    const map<PlayerTypes, vector<Effects>>LEVEL_EFFECT_RELATION = {
       {OUTLAW, {HIDDEN_ATTACK, AGILITY_BOOST, POISON}},
       {WARRIOR, {RUSH_TO_ACTION, NO_EFFECT, STRENGTH_BOOST}},
       {BARBARIAN, {RAGE, NO_EFFECT, ENDURANCE_BOOTS}}
    };

    /// @brief Соотношение тегов игроков с соответствующими бонусами (постэффектами) на каждом из трёх уровней
    const map<PlayerTypes, vector<PostEffects>>LEVEL_POSTEFFECT_RELATION = {
        {OUTLAW, {NO_POST_EFFECT, NO_POST_EFFECT, NO_POST_EFFECT}},
        {WARRIOR, {NO_POST_EFFECT, SHIELD, NO_POST_EFFECT}},
        {BARBARIAN, {NO_POST_EFFECT, STONE_SKIN, NO_POST_EFFECT}}
    };

}

#endif // PLAYERDATA_ONCE