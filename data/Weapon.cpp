#include "Weapon.h"

#include <map>

using namespace std;

/**
 * @brief Конструктор по трём числам
 *
 * @param d Наносимый урон
 * @param dt Тип урона
 * @param name Видимое имя
 */
Weapon::Weapon(int d, DamageType dt, string name) : damage(d), damage_type(dt), weapon_name(name) {};

/**
 * @brief Создаёт оружие для видимости (т. е. по урону и отображаемому названию)
 *
 * @param damage Наносимый урон
 * @param name Видимое название
 *
 * #### Example:
 *
 * Используется только для создания оружия врагам. Их урон зачастую уменьшенный:
 * @see Enemy::set_defaults()
 *
 */
Weapon::Weapon(int damage, string name) {
    this->damage = damage;
    this->damage_type = NO_DAMAGE;
    this->weapon_name = name;
}

/// @brief Соотношение типов урона с их видимыми строковыми значениями
const map<DamageType, string> DAMAGE_TYPES_VISIBLE_NAMES = {
    {CHOPPING, "Chopping"}, {CRUSHING, "Crushing"}, {STABBING, "Stabbing"}
};

/**
 * @brief Получает видимое имя типа урона
 *
 * @return string Видимое имя типа урона
 */
string Weapon::get_visible_damage_type() {
    return DAMAGE_TYPES_VISIBLE_NAMES.at(damage_type);
};