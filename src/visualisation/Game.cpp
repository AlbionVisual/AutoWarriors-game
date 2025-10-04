#include "Game.h"

#include <iostream>

using std::cout;
using std::string;
using std::to_string;

/**
 * @brief Создание новой игры. Вызывается функция инициализатор Game::init()
 *
 */
Game::Game() {
    init();
}

/**
 * @brief Перерисовывает консоль и создаёт игрока с изображением игры, вешает функцию-принтер
 *        всем персонажам
 *
 * #### Example:
 *
 * ```
 * Game game = Game(); // Неявно вызывается init()
 * game.start(); // Запуск игры
 * game.init(); // Переинициализация игры
 * game.start(); // Перезапуск
 * ```
 *
 */
void Game::init() {
    clear_console();

    // Создание игрока и рисовальщика игры
    player = Player(ask_for_class());
    game = GameDrawer(player);

    // Привязка функции вывода сообщений к статическому члену персонажей
    Pawn::external_printer = bind(
        &GameDrawer::add_message,
        &game,
        placeholders::_1,
        placeholders::_2,
        placeholders::_3
    );

    game.redraw_console();
}

/**
 * @brief Ждёт указанное время занимая единственный поток
 *
 * @param seconds Время ожидания в секундах
 *
 * #### Example:
 * ```
 * Game game = Game();
 * game.start();
 * game.wait(2); // Перезапуск через 2 секунды
 * game.init();
 * game.start();
 * ```
 *
 */
void Game::wait(unsigned int seconds) {
    this->game.freeze(seconds);
}

/**
 * @brief Запуск игры. Цилкиться 5 раз, запуская функцию проведения боя со случайным противником
 *        Game::play_round(bool)
 *
 * #### Example:
 *
 * ```
 * Game game = Game();
 * game.start();
 * ```
 *
 */
void Game::start() {
    // Запускаем 5 игр
    int code;
    for (int i = 0; i < 5; i++) {
        code = play_round(i == 4);
        if (code == 1) break; // Если проиграли, то выходим
    }

    // Вывод сообщения (победил / проиграл)
    cout << "\n\n";
    if (code == 1) {
        colorize_text(RED_WHITE);
        cout << "\tYou lost\t";
        colorize_text();
    }
    else {
        colorize_text(GREEN_WHITE);
        cout << "\tYou won!\t";
        colorize_text();
    }
    cout << "\n\n";
}


/**
 * @brief Запускает бой со случайным врагом, а затем проводит диалог по улучшеиям
 *
 * @param is_last Последний ли бой (спрашивать ли смену орудия у пользователя)
 * @return 0 - Бой закончился победой игрока
 * @return 1 - Бой закончился поражением игрока
 *
 * #### Example:
 *
 * Используется только в Game::start()
 *
 * Проиграем 5 боёв:
 * ```
 * for (int i = 0; i < 5; i++) {
 *     code = play_round(game, player, enemy_list, enemies_amount, i == 4);
 *     if (code == 1) break;
 * }
 * ```
 *
 */
int Game::play_round(bool is_last) {
    int code = play_fight();
    if (code == 1) return 1;

    player.reset_player();
    game.redraw_console();

    if (player.get_level() < 3) {
        cout << "You can now upgrade your level...\n";
        player.add_level(ask_for_class());
        game.redraw_console();
    }

    if (!is_last) {
        cout << "Choose a weapon:\n";
        Weapon reward = enemy_list[enemies_amount - 1].get_reward();
        string weapon1 = player.active_weapon.weapon_name + ", damage: " + to_string(player.active_weapon.damage);
        int amount_of_tabs = 3 - int(weapon1.length() / 8);
        while (amount_of_tabs-- > 0) weapon1 += '\t';
        cout << weapon1 << "type \"0\" (current)\n";
        string weapon2 = reward.weapon_name + ", damage: " + to_string(reward.damage);
        amount_of_tabs = 4 - int(weapon2.length() / 8);
        while (amount_of_tabs-- > 0) weapon2 += '\t';
        cout << weapon2 << "type \"1\"\n";

        int ans;
        cin >> ans;
        if (ans == 1) {
            player.active_weapon = reward;
        }
    }
    return 0;
}

/**
 * @brief Проиграть сам бой
 *
 * @return 0 - Бой закончился победой игрока
 * @return 1 - Бой закончился поражением игрока
 *
 * #### Example:
 *
 * Используется только в Game::play_round(bool)
 */
int Game::play_fight() {

    Enemy& enemy1 = enemy_list[enemies_amount++] = Enemy(); // Создаём нового врага и добавляем в конец списка

    /// @brief Вспомогательные переменные для более простой смены направления удара
    Pawn* pl1 = &enemy1, * pl2 = &player, * temp;
    bool attacking_to_left = true;
    if (player.get_agility() >= enemy1.get_agility()) { // Меняем местами врага и игрока, если враг не имеет высшую ловкость
        pl1 = &player;
        pl2 = &enemy1;
        attacking_to_left = false;
    }

    // Отрисовка
    game.add_enemy(enemy1);
    game.change_attack_direction(attacking_to_left);
    game.redraw_console();

    // Ходы
    int move_number = 0;
    while (pl1->get_health() > 0 && pl2->get_health() > 0) {
        pl2->attacked_by(*pl1, move_number);
        game.redraw_console();
        if (pl1->get_health() == 0 || pl2->get_health() == 0) break; // Если один умер, завершаем бой
        // Иначе меняем местами игрока и врага (в переменных)
        temp = pl1;
        pl1 = pl2;
        pl2 = temp;
        // А также перерисовываем экран
        attacking_to_left = !attacking_to_left;
        game.change_attack_direction(attacking_to_left);
        game.redraw_console();
        move_number += 1;
    }
    if (player.get_health() == 0) { // Если умер игрок
        return 1;
    }
    return 0; // Иначе умер враг
}

/**
 * @brief Спрашивает пользователя выбрать класс игрока
 *
 * @return Players::PlayerTypes - Класс, выбранный пользователем
 *
 * #### Example:
 *
 * Используется в Game::play_round(bool)
 * в качестве подъёма уровня, а также в main()
 * в качестве выбора начального персонажа
 */
Players::PlayerTypes Game::ask_for_class() {
    cout << "Choose player class (Outlaw (O), Warrior (W), Barbarian (B)): ";
    string player_class = "";
    cin >> player_class;
    while (get_class_from_string(player_class) == Players::NO_PLAYER_SELECTED) {
        cout << "Try again! Choose player class (Outlaw (O), Warrior (W), Barbarian (B)): ";
        cin >> player_class;
    }
    return get_class_from_string(player_class);
}

/**
 * @brief Преобразует строку в тег игрока. Смотрится только первая буква, причём не зависимо
 *        от регистра, поэтому 'woutlaw' будет преобразовано в Players::WARRIOR
 *
 * @param player_class Строка с выбранным классом игрока
 * @return Players::PlayerTypes - Класс персонажа
 * @return Players::NO_PLAYER_SELECTED - Если не было совпадений
 *
 */
Players::PlayerTypes Game::get_class_from_string(const string& player_class) {
    if (player_class.length() > 0) {
        switch (player_class[0])
        {
        case 'O':
        case 'o':
            return Players::OUTLAW;
        case 'B':
        case 'b':
            return Players::BARBARIAN;
        case 'W':
        case 'w':
            return Players::WARRIOR;
        default:
            return Players::NO_PLAYER_SELECTED;
        }
    }
    else return Players::NO_PLAYER_SELECTED;
}