#ifndef WEAPON_ONCE
#define WEAPON_ONCE

#include <string>
using namespace std;

/// @brief Теги типов уронов, а также заглушка NO_DAMAGE
enum DamageType { NO_DAMAGE, CHOPPING, CRUSHING, STABBING }; // Рубящий, Дробящий, Колющий

/**
 * @brief Структура для хранения характеристик оружия
 *
 */
struct Weapon {
    Weapon(int d, DamageType dt, string name);
    Weapon(int damage, string name = "hand");
    Weapon() {};
    string get_visible_damage_type();

    /// @brief Урон, наносимый оружием
    int damage = -1;

    /// @brief Тип урона, наносимый оружием
    DamageType damage_type = NO_DAMAGE;

    /// @brief Видимое строковое имя оружия 
    string weapon_name = "hand";
};

// Константы, задающие стандартные используемые экземпляры оружий

const Weapon SWORD = Weapon(3, CHOPPING, "Sword");
const Weapon CLUB = Weapon(3, CRUSHING, "Club");
const Weapon DAGGER = Weapon(2, STABBING, "Dagger");
const Weapon AXE = Weapon(4, CHOPPING, "Axe");
const Weapon SPEAR = Weapon(3, STABBING, "Spear");
const Weapon L_SWORD = Weapon(10, CHOPPING, "Legendary sword");


#endif // WEAPON_ONCE