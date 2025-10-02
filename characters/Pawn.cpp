#include "Pawn.h"

printer_type Pawn::external_printer; // Инициализация статичного поля)

/**
 * @brief Создаёт новый экземпляр структуры с заданными начальными данными
 *
 * @param s Сила (прибавляется к урону)
 * @param a Ловкость (учитывается при ударе, даёт вероятность промоха)
 * @param e Выносливость (каждый бой прибавляет к уровню здоровья это количество поинтов)
 *
 * #### Example:
 *
 * Используется при создании врагов:
 * @see Enemy::set_defaults()
 * , когда создаются константы (возможно заметить это будет затруднительно, но в других местах
 * нет примера):
 * @see SPECIFICATIONS
 * . Поле характеристики при помощи оператора = копируется по значениям в поле только что
 * созданного объекта класса `Enemy`.
 *
 */
pawn_specifications::pawn_specifications(int s, int a, int e) : strength(s), agility(a), endurance(e) {};

/**
 * @brief Создаёт новый экземпляр структуры со случайными значениями полей
 *
 * #### Example:
 *
 * Используется неявно при создании персонажей:
 * @see Player::Player()
 * . Поле характеристики просто не заполняется, поэтому вызывается этот конструктор по
 * умолчанию
 *
 */
pawn_specifications::pawn_specifications() : strength(rand() % 3 + 1), agility(rand() % 3 + 1), endurance(rand() % 3 + 1) {};

/**
 * @brief Сумма структур ведётся по полям (поэлементно)
 *
 * @param other Другая структура, которая прибавляется
 * @return pawn_specifications Новый объект, созданный как сумма
 *
 * #### Example:
 *
 * Используется при создании врагов:
 * @see Enemy::set_defaults()
 * . Поле характеристики при помощи оператора = копируется по значениям в поле только что
 * созданного объекта класса `Enemy`.
 *
 * ```
 * pawn_specifications(1,2,3) + pawn_specifications(0,7,3);
 * ```
 *
 * В результате ожидается:
 *
 * ```
 * { strength: 1, agility: 8, endurance: 6}
 * ```
 *
 */
pawn_specifications pawn_specifications::operator+(const pawn_specifications& other) const {
    return pawn_specifications(this->strength + other.strength, this->agility + other.agility, this->endurance + other.endurance);
}

/**
 * @brief Основная функция персонажа для проведения боя. Тут вычисляется шанс попадания удара
 *        (при помощи полей specifications.agility у первого и второго персонажей), затем применяются эффекты по-очереди при помощи переписываемых методов
 *        @see apply_damage(const Pawn&, int, int)
 *      , @see get_extra_specifications(Pawn&, int)
 *        и аналогичных у
 *        @see Player
 *
 * @param attacker Тот кто атакуюет персонажа, доступ к которому будет получен через this
 * @param move_number Номер хода для передачи в эффекты
 *
 * #### Example:
 *
 * Из
 * @see main.cpp
 * ```
 * pl2->attacked_by(*pl1, move_number);
 * ```
 */
void Pawn::attacked_by(Pawn& attacker, int move_number) {
    pawn_specifications& shield = this->specifications;
    int chance = rand() % (attacker.specifications.agility + shield.agility) + 1;
    pawn_specifications extra_specifications = attacker.get_extra_specifications(*this, move_number); // Применение всех эффектов у атакующего
    if (chance > shield.agility) { // Если атакующий не промахнулся
        apply_damage(attacker, extra_specifications.strength, move_number); // Применение всех эффектов на урон у цели
    }
    else { // Иначе вывести сообщение о том, что атака промахнулась и завершить тем самым ход
        if (external_printer) external_printer("Attack missed!\n", 1, BLACK_WHITE);
    }
};

/**
 * @brief Применение урона. Расширяется для подсчёта результата работы постэффектов
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
 * Расширяется в классах:
 * @see Player
 * и @see Enemy
 * . Сами методы:
 * @see Player::apply_damage(const Pawn&, int, int)
 * , @see Enemy::apply_damage(const Pawn&, int, int)
 *
 */
void Pawn::apply_damage(const Pawn& damager, int extra_damage, int move_number) {
    health -= damager.specifications.strength + damager.active_weapon.damage + extra_damage;
    if (health < 0) health = 0;
}

/**
 * @brief Получить здоровье
 *
 * @return int - текущее значение здоровья
 */
int Pawn::get_health() { return health; };

/**
 * @brief Получить выносливость
 *
 * @return int - текущее значение выносливости
 */
int Pawn::get_agility() { return specifications.agility; };
