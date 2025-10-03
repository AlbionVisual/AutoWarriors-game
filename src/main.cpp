/**
 * @file main.cpp
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Запуск игры Auto Warriors Game
 * @date 2025-10-04
 *
 */

#include "./visualisation/Game.h"
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {

    srand(static_cast<unsigned int>(time(0)));

    Game game = Game();
    game.start();
    // game.wait(2); // Перезапуск через 2 секунды
    // game.init();
    // game.start();
    cin.ignore();
    cin.get();
    return 0;
}