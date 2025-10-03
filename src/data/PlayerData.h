/**
 * @file PlayerData.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Начальные данные, которые вполне не сложно вынести в json. Могут возникнуть
 *        трудности только с добавлением своих новых эффектов и классов игрока (т. к. там
 *        меняется вся логика в их поведени)
 *
 *        Описывает в пространстве имён Players
 *
 * @date 2025-10-01
 *
 */

#ifndef PLAYERDATA_ONCE
#define PLAYERDATA_ONCE

#include "../characters/Player.h"
#include "PostEffects.h"

using std::map;
using std::string;
using std::vector;

/**
 * @brief Пространство имён для определения данных о предопределённых классах игрока. Все
 *        соотношения тут можно настраивать под нужды гейм-дизайнера
 *
 */
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
    const map<PlayerTypes, vector<Effects>>EFFECT_RELATION = {
       {OUTLAW, {HIDDEN_ATTACK, AGILITY_BOOST, POISON}},
       {WARRIOR, {RUSH_TO_ACTION, NO_EFFECT, STRENGTH_BOOST}},
       {BARBARIAN, {RAGE, NO_EFFECT, ENDURANCE_BOOTS}}
    };

    /// @brief Соотношение тегов игроков с соответствующими бонусами (постэффектами) на каждом из трёх уровней
    const map<PlayerTypes, vector<PostEffects>>POSTEFFECT_RELATION = {
        {OUTLAW, {NO_POST_EFFECT, NO_POST_EFFECT, NO_POST_EFFECT}},
        {WARRIOR, {NO_POST_EFFECT, SHIELD, NO_POST_EFFECT}},
        {BARBARIAN, {NO_POST_EFFECT, STONE_SKIN, NO_POST_EFFECT}}
    };

}

#endif // PLAYERDATA_ONCE