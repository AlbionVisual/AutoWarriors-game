/**
 * @file EnemyData.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Начальные данные, которые вполне не сложно вынести в json. Могут возникнуть
 *        трудности только с добавлением своих новых эффектов и врагов (т. к. там меняется вся
 *        логика в их поведени)
 *
 *        Описывает в пространстве имён Enemies
 *
 * @date 2025-10-01
 *
 */

#include "../characters/Pawn.h"
#include "../characters/Enemy.h"
#include "Effects.h"
#include "PostEffects.h"

using std::map;
using std::vector;
using std::string;

/**
 * @brief Пространство имён для определения данных о предопределённых врагах. Все соотношения
 *        тут можно настраивать под нужды гейм-дизайнера
 *
 */
namespace Enemies {

    /// @brief Массив типов врагов для итерирования по ним
    constexpr Types TYPES_VALUES[] = { GOBLIN, SKELETON, SLIME, GHOST, GOLEM, DRAGON };

    /// @brief Количество типов врагов
    const short TYPES_AMOUNT = 6;

    /// @brief Соотношение тегов с их строковыми именами
    const map<Types, string> VISIBLE_NAMES = {
        {GOBLIN, "Goblin"}, {SKELETON, "Skeleton"}, {SLIME, "Slime"},
        {GHOST, "Ghost"},{GOLEM, "Golem"}, {DRAGON, "Dragon"}
    };

    /// @brief Соотношение тегов врагов с их начальными характеристиками
    const map<Types, pawn_specifications> SPECIFICATIONS = {
        { GOBLIN, {1,1,1}}, {SKELETON, {2,2,1}},{SLIME, {3,1,2}},
        {GHOST, {1,3,1}},{GOLEM, {3,1,3}},{DRAGON,{3,3,3}}
    };

    /// @brief Соотношение тегов врагов с их начальными значениями здоровья
    const map<Types, int> INITIAL_HEALTHES = {
        {GOBLIN, 5}, {SKELETON, 10}, {SLIME, 8}, {GHOST, 6}, {GOLEM, 10}, {DRAGON, 20}
    };

    /// @brief Соотношение тегов врагов с наградами после их уничтожения
    const map<Types, Weapon> REWARDS = {
        {GOBLIN, DAGGER}, {SKELETON, CLUB}, {SLIME, SPEAR},
        {GHOST, SWORD}, {GOLEM, AXE}, {DRAGON, L_SWORD}
    };

    /// @brief Соотношение тегов врагов с их фактическими значениями урона от оружия, не зависимо от самого оружия
    const map<Types, int> WEAPON_DAMAGE = {
        {GOBLIN, 2}, {SKELETON, 2}, {SLIME, 1}, {GHOST, 3}, {GOLEM, 1}, {DRAGON, 4}
    };

    /// @brief Соотношение тегов с эффектами, которые по сути будут способностями перед ударом
    const map<Types, Effects>EFFECT_RELATION = {
        {GOBLIN, NO_EFFECT}, {SKELETON, NO_EFFECT}, {SLIME, NO_EFFECT},
        {GHOST, HIDDEN_ATTACK}, {GOLEM, NO_EFFECT}, {DRAGON, FIRE_BREATHING}
    };

    /// @brief Соотношение тегов с постэффектами, которые по сути будут способностями при ударах
    const map<Types, PostEffects>POSTEFFECT_RELATION = {
        {GOBLIN, NO_POST_EFFECT}, {SKELETON, DOUBLE_DAMAGE_ON_CRUSHING}, {SLIME, SLIMED_DAMAGE},
        {GHOST, NO_POST_EFFECT}, {GOLEM, STONE_SKIN}, {DRAGON, NO_POST_EFFECT}
    };

}