
#include "ConsoleUtils.h"

#ifdef _WIN32

// Windows
#define WIN32_LEAN_AND_MEAN
#define _USING_V110_SDK71_

#include <windows.h>

/**
 * @brief Очищение консоли
 *
 * Функция определяется по-разному для работы и с терминалами Windows (используются
 * `FillConsoleOutputCharacter` с пробелами, `HANDLE` и `SetConsoleCursorPosition`), и с
 * другими (при помощи escape-последовательности "\033[2J\033[H")
 */
void clear_console() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = { 0, 0 };
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwBufferSize = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, ' ', dwBufferSize, coord, &dwCharsWritten);
    FillConsoleOutputAttribute(hConsole, 0x1 | 0x2 | 0x4, dwBufferSize, coord, &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, coord);
}

/*
Синий:      0x1
Зелёный:    0x2
Красный:    0x4
=> Жёлтый:  0x2 | 0x4
Для фонов добавляется 0 в последнем разряде (фон также примешивается к цвету текста)
*/

/**
 * @brief После выполнения этой функции весь выводимый текст будет заданного цвета
 *
 * Функция адаптирована для работы и с терминалами Windows (используются
 * `SetConsoleTextAttribute` и `HANDLE`), и с другими (при помощи escape-последовательностей,
 * например "\033[0m")
 *
 * @param color новый цвет консоли. По умолчанию ч/б, т.е. для отмены можно вызвать функцию без аргумента
 *
 * #### Example:
 *
 * ```
 * colorize_text(BLACK_GREEN);
 * std::cout << "Green text";
 * colorize_text();
 * std::cout << " + normal text" << std::endl;
 * ```
 *
 * @see Много примеров использования в `./GameDrawer.cpp`
 */
void colorize_text(ConsoleColor color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color)
    {
    case BLACK_WHITE:
        SetConsoleTextAttribute(hConsole, 0x1 | 0x2 | 0x4);
        break;
    case RED_WHITE:
        SetConsoleTextAttribute(hConsole, 0x40 | 0x1 | 0x2 | 0x4);
        break;
    case GREEN_WHITE:
        SetConsoleTextAttribute(hConsole, 0x20 | 0x1 | 0x2 | 0x4);
        break;
    case BLACK_GREEN:
        SetConsoleTextAttribute(hConsole, 0x2);
        break;
    case BLACK_RED:
        SetConsoleTextAttribute(hConsole, 0x4);
        break;
    default:
        SetConsoleTextAttribute(hConsole, 0x1 | 0x2 | 0x4);
        break;
    }
}

#else
// Linux/macOS
#include <iostream>
void colorize_text(ConsoleColor color = BLACK_WHITE);
void clear_console() {
    colorize_text();
    std::cout << "\033[2J\033[H";
}

/*
Цвета задаются с помощью последовательности \033[<код_цвета_фона>m или
\033[<код_цвета_текста>;<код_цвета_фона>m.

Назначение	Код
Сброс всех атрибутов (цвет по умолчанию)	\033[0m
Цвета текста (Foreground)
Черный	30
Красный	31
Зеленый	32
Желтый	33
Синий	34
Цвета фона (Background)
Черный фон	40
Красный фон	41
Зеленый фон	42
Желтый фон	43
Синий фон	44
*/

void colorize_text(ConsoleColor color) {
    switch (color)
    {
    case BLACK_WHITE:
        std::cout << "\033[0m";
        break;
    case RED_WHITE:
        std::cout << "\033[41m";
        break;
    case GREEN_WHITE:
        std::cout << "\033[42m";
        break;
    case BLACK_GREEN:
        std::cout << "\033[32;40m";
        break;
    case BLACK_RED:
        std::cout << "\033[31;40m";
        break;
    default:
        std::cout << "\033[0m";
        break;
    }
}

#endif