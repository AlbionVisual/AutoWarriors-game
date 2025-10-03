/**
 * @file ConsoleUtils.h
 * @author Vlad Korzun (vlad.korzun.minsk@gmail.com)
 * @brief Кроссплатформенные функции для работы с консолью, а также перечисление с
 *        поддерживаемыми цветами для консоли. Функции имеют одно объявление, но разные
 *        реализации в зависимости от используемой ОС
 * @date 2025-10-01
 *
 */

#ifndef CONSOLE_ONCE
#define CONSOLE_ONCE

 /**
  * @brief Выбор цвета для консоли (цвет фона _ цвет текста)
  */
enum ConsoleColor { BLACK_WHITE, RED_WHITE, GREEN_WHITE, BLACK_GREEN, BLACK_RED };

void colorize_text(ConsoleColor color = BLACK_WHITE);

void clear_console();

#endif // CONSOLE_ONCE