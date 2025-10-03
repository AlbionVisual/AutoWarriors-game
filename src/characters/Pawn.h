/**
 * @file Pawn.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Здесь описывается абстрактный класс любого персонажа в игре, тип-функции стороннего
 *        вывода текста и структура для передачи и хранения динамических данных о персонаже
 * @date 2025-10-02
 *
 */

#ifndef PAWN_ONCE
#define PAWN_ONCE

#include "../data/Weapon.h"
#include "../utils/ConsoleUtils.h"
#include <map>
#include <functional>

 /**
  * @brief Тип функции вывода сообщения на экран, для сохранения в качестве статичного поля и
  *        последующего использования. Подробно описывается в:
  *        GameDrawer::add_message(string, int, ConsoleColor)
  *
  */
using printer_type = std::function<void(std::string, int, ConsoleColor)>;

/**
 * @brief Структура для хранения и передачи статичных значений характеристик персонажей
 *
 * `strength` - сила (прибавляется к урону)
 *
 * `agility` - ловкость (учитывается при ударе, даёт вероятность промоха)
 *
 * `endurance` - выносливость (каждый бой прибавляет к уровню здоровья это количество поинтов)
 *
 */
struct pawn_specifications {
    pawn_specifications(int s, int a, int e);
    pawn_specifications();
    pawn_specifications operator+(const pawn_specifications& other) const;

    /// @brief Сила; прибавляется к урону
    int strength;

    /// @brief Ловкость; учитывается при ударе, даёт возможность промоха
    int agility;

    /// @brief Выносливость; каждый бой прибавляет к уровню здоровья это количество поинтов
    int endurance;
};

/**
 * @brief Абстрактный класс любого персонажа в игре, будь то игрок или враг.
 *
 */
class Pawn {

public:

    /// @brief Статичное поле для хранения доступа к функции-принтеру цветных сообщений на экране
    static printer_type external_printer;

    /// @brief Стандартные характеристики персонажа. Каждый удар идёт расчёт дополнительных характеристик независимо, за исключением персистентных эффектов
    pawn_specifications specifications;

    /// @brief Здоровье персонажа
    int health;

    /// @brief Активное оружие
    Weapon active_weapon;

    /**
     * @brief Конструктор по умолчанию для инициализации в качестве поля. Нельзя использовать
     *        объект-результат этого конструктора
     *
     */
    Pawn() {};

    /**
     * @brief Получает дополнительные данные, которые нужно именно добавить при атаке к характеристикам
     *
     * @param target На кого идёт удар этим персонажем
     * @param move_number Номер хода
     * @return pawn_specifications - Характеристики, которые нужно добавить перед ударом
     *
     * #### Example:
     *
     * Используется только в Pawn::attacked_by(Pawn&, int)
     *
     * Также переписывается в Player::get_extra_specifications(const Pawn&, int)
     * и Enemy::get_extra_specifications(const Pawn&, int)
     */
    virtual pawn_specifications get_extra_specifications(const Pawn& target, int move_number) = 0;
    void attacked_by(Pawn& attacker, int move_number);
    virtual void apply_damage(const Pawn& damager, int extra_damage, int move_number);
    int get_health();
    int get_agility();
};

#endif // PAWN_ONCE