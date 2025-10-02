/**
 * @file ConsoleUtils.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Кроссплатформенные функции для работы с консолью
 * @date 2025-10-01
 *
 */

#ifndef CONSOLE_ONCE
#define CONSOLE_ONCE

 /**
  * @brief Выбор цвета для консоли (цвет фона_цвет текста)
  */
enum ConsoleColor { BLACK_WHITE, RED_WHITE, GREEN_WHITE, BLACK_GREEN, BLACK_RED };

void colorize_text(ConsoleColor color = BLACK_WHITE);

void clear_console();

#endif // CONSOLE_ONCE