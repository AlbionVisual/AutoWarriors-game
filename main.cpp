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

    return 0;
}