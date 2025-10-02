
#include "GameDrawer.h"

#include "../utils/ConsoleUtils.h"
#include <chrono>
#include <thread>

/**
 * @brief Создание нового экземпляра класса:
 *        @see GameDrawer
 *
 * @param pl Игрок; сохраняется сразу и в кэшированную версию
 */
GameDrawer::GameDrawer(Player& pl) : player_state(&pl), cashed_player_state(Player(pl)) {};

/**
 * @brief Останавливает единственный поток выполнения кода на определённое время
 *
 * @param seconds Количество секунд для простаивания
 *
 * #### Example:
 *
 * Используется только внутри:
 * @see GameDrawer::redraw_console()
 *
 */
void GameDrawer::freeze(size_t seconds) { this_thread::sleep_for(chrono::seconds(seconds)); };

/**
 * @brief Добавляет врага и обновляет кэшированное состояние списка врагов
 *
 * @param en Новый враг
 *
 * #### Example:
 *
 * Используется только в:
 * @see play_fight(GameDrawer&, Player&, Enemy*, unsigned short&)
 *
 */
void GameDrawer::add_enemy(Enemy& en) {
    if (enemies_amount > 0) cashed_enemies_state[enemies_amount - 1] = *active_enemy_state;
    active_enemy_state = &en;
    cashed_enemies_state[enemies_amount++] = en;
}

/**
 * @brief Изменяет направление атаки (изменяется только направление стрелки)
 *
 * @param new_direction Новое направление (true - влево, false - вправо)
 *
 * #### Используется только в:
 * @see play_fight(GameDrawer&, Player&, Enemy*, unsigned short&)
 *
 */
void GameDrawer::change_attack_direction(bool new_direction) { attack_left = new_direction; };

/**
 * @brief Выводит число с соответствующим цветом:
 *        зелёным (если число увеличилось с предыдущего кадра),
 *        красным (если число уменьшилось)
 *
 * @param n1 новое переданное число
 * @param n2 старое, кэшированное значение того же поля
 * @return true - была окраска текста
 * @return false - не было окраски текста
 *
 * #### Example:
 *
 * ```
 * std::cout << "Health: ";
 * colored_print(player.health, cashed_player.health);
 * std::cout << "\nEndurance: ";
 * ```
 *
 * Используется только в:
 * @see GameDrawer::redraw_console
 *
 */
bool GameDrawer::colored_print(int n1, int n2) {
    bool selection_enabled = false;
    if (n1 > n2) { // Если изменение в положительную сторону
        colorize_text(GREEN_WHITE);
        selection_enabled = true;
    }
    else if (n2 > n1) { // Или если изменения в отрицательную сторону
        colorize_text(RED_WHITE);
        selection_enabled = true;
    }
    else colorize_text(); // Иначе Оставляем без цвета
    std::cout << to_string(n1);
    colorize_text();
    return selection_enabled;
};

/**
 * @brief Очищает консоль, а затем выводит сохранённую информацию каждый раз одинаково.
 *        Изменённая информация сразу окрашивается отличающимся цветом, а затем (через секунду)
 *        перерисовывается обратно без подсветки. Все активные сообщения (с временем большим 0)
 *        отрисовываются после, остальные удаляются, а время уменьшается на 1. Единтсвенная
 *        объёмная функция
 *
 * #### Example:
 *
 * Обильно используется в:
 * @see ./main.cpp
 *
 */
void GameDrawer::redraw_console() {
    clear_console();
    // Если есть какие-то новые данные, эта переменная их оттрекает и заставит окно ещё раз
    // перерисоваться, чтобы убрать выделение до следующего кадра. Когда вызывается
    // перерисовка следующего кадра, эта переменная будет всегда false и третьей перерисовки
    // не будет
    bool selection_enabled = false;
    cout << "\tAutoWarriors game\n"; // Начало информационной части консоли
    cout << "Entity\tClass\t\tHealth\tStrength\tAgility\tEndurance\tWeapon: damage\n";
    for (short i = enemies_amount; i >= 0; i--) { // Обработка статистики для каждой сущности
        Pawn* pawn = player_state;
        Pawn* cashed_pawn = &cashed_player_state;
        if (i == enemies_amount) {                // Первым выводим игрока
            pawn = player_state;
        }
        else {
            cashed_pawn = &cashed_enemies_state[i];
            if (i == enemies_amount - 1) {        // Затем активного врага
                pawn = active_enemy_state;
            }
            else {                                // А потом всех остальных
                pawn = &cashed_enemies_state[i];
            }
        }
        string pawn_type = (i == enemies_amount) ? "Player" : "Enemy";
        string visible_name = (i == enemies_amount) ? player_state->get_visible_name() : (i == enemies_amount - 1) ? active_enemy_state->get_visible_name() : cashed_enemies_state[i].get_visible_name();
        cout << pawn_type + "\t" + visible_name + "\t"; // Начало вывода статистики одной сущности
        if (visible_name.length() < 8) cout << "\t";
        selection_enabled = colored_print(pawn->health, cashed_pawn->health) | selection_enabled;
        cout << "\t";
        selection_enabled = colored_print(pawn->specifications.strength, cashed_pawn->specifications.strength) | selection_enabled;
        cout << "\t\t";
        selection_enabled = colored_print(pawn->specifications.agility, cashed_pawn->specifications.agility) | selection_enabled;
        cout << "\t";
        selection_enabled = colored_print(pawn->specifications.endurance, cashed_pawn->specifications.endurance) | selection_enabled;
        cout << "\t\t" + pawn->active_weapon.weapon_name + ": ";
        selection_enabled = colored_print(pawn->active_weapon.damage, cashed_pawn->active_weapon.damage) | selection_enabled;
        cout << "\n";                                   // Конец вывода статистики одной сущности
    }
    cout << "\n";
    if (enemies_amount > 0) { // Если враги есть, то рисуем бой
        string direction_arrow = attack_left ? "<--" : "-->";
        cout << "\t\t";
        if (player_state->health < cashed_player_state.health) { // Если есть изменения в здоровье игрока, то отображаем это слева
            colorize_text(RED_WHITE);
            cout << "-" + to_string(cashed_player_state.health - player_state->health);
            colorize_text();
            selection_enabled = true; // Тоже просим перерисовать консоль, чтобы через секунду это выделение пропало
        }
        cout << "\tYou\t" + direction_arrow + "\t" + active_enemy_state->get_visible_name() + "\t";
        int& last_enemy_health = active_enemy_state->health;
        int& last_cashed_enemy_health = cashed_enemies_state[enemies_amount - 1].health;
        if (last_enemy_health < last_cashed_enemy_health) { // Если есть изменения здоровья врага, то тоже отрисовываем это рядом с врагом
            colorize_text(GREEN_WHITE);
            cout << "-" + to_string(last_cashed_enemy_health - last_enemy_health);
            colorize_text();
            selection_enabled = true;
        }
        cout << "\n\n";
    }                         // Конец отрисовки боя

    // Отрисовка всех активных сообщений (они не учавствуют в автоматической отмене
    // перерисовки, т.к. имеют собсвтенный таймер, поэтому selection_enabled не устанавливается
    // в true, но изменяется оставшееся время)
    for (auto message_iter = message_list.begin(); message_iter != message_list.end(); message_iter++) {
        Message& message = *message_iter;
        if (message.seconds <= 0) { // Проверка времени (удаляем, если время истекло)
            auto temp = message_iter--;
            message_list.erase(temp);
            continue;
        };
        colorize_text(message.color); // Отрисовываем окрашенный текст
        cout << message.text;
        colorize_text();
        message.seconds -= 1; // Уменьшаем оставшееся время
    }


    if (selection_enabled) { // Если был запрос на отмену временного выделения
        sync_cache();
        freeze(); // то вызываем перерисовку после небольшой паузы
        redraw_console();
    }
    else freeze(); // Или просто ждём
}

/**
 * @brief Обновляет кэшированные данные (сохраняет текущие в переменные кэша)
 *
 * #### Example:
 *
 * Используется один раз в:
 * @see GameDrawer::redraw_console()
 *
 */
void GameDrawer::sync_cache() {
    cashed_enemies_state[enemies_amount - 1] = *active_enemy_state;
    cashed_player_state = *player_state;
}

/**
 * @brief Функция передаваемая в качестве external_printer в функции-эффекты и постэффекты.
 *        Отвечает за сохранение новых сообщений в поле. Не переносит новые сообщения на новую
 *        строку, это нужно делать вручную в конце любого единого сообщения (это позволяет
 *        выводить одно сообщение разными цветами)
 *
 * @param text Текст сообщения
 * @param seconds Время отображения на экране сообщения
 * @param color Цвет отображаемого сообщения
 *
 * #### Example:
 *
 * Можно просто передать текст и время, тогда текст выведется как обычный, а можно окрашивать
 * текст, причём не обязательно полностью:
 *
 * ```
 * game.add_message("Some text to be printed in green!\n", 1, BLACK_GREEN);
 * game.freeze(2);
 * game.add_message("Player has ", 2);
 * game.add_message(std::to_string(player.health), 2, BLACK_GREEN);
 * game.add_message(" health points\n", 2);
 * ```
 *
 * Тут первое сообщение будет выведено на секунду, затем пропадёт. А второе сообщение
 * выведется в совокупности из трёх экземпляров Message как один, но покрашенный в середине.
 * Предполагаемые выводы (без учёта цвета):
 *
 * В первую секунду:
 * ```
 * <stats>
 *
 *      You <-- Goblin
 *
 * Some text to be printed in green!
 * ```
 *
 * В третюю секунду:
 * ```
 * <stats>
 *
 *      You <-- Goblin
 *
 * Player has 12 health points
 * ```
 *
 * Использование можно посмотреть в качестве переданной функции `external_printer` в
 * функциях-эффектах, постэффектах, а также их вызова в
 * @see Player::get_extra_specifications(Pawn&, int)
 * , @see Enemy::get_extra_specifications(Pawn&, int)
 * , @see Player::apply_damage(const Pawn&, int, int)
 * и @see Enemy::apply_damage(const Pawn&, int, int)
 *
 */
void GameDrawer::add_message(std::string text, int seconds, ConsoleColor color) {
    message_list.push_back(Message(text, seconds, color));
};