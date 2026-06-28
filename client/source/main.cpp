/**
* @file main.cpp
* @brief Главный файл программы.
* @details Этот файл содержит функцию main, которая инициализирует интерфейс пользователя и запускает программу.
* @date 25.06.2026
* @version 1.0
* @authors Какулин А.А.
* @copyright ИБСТ ПГУ
*/
#include "modules/ui.h"
#include <iostream>

/**
 * @brief Главная функция программы.
 * @details Инициализирует объект UserInterface и запускает его. Обрабатывает все исключения, возникающие во время выполнения программы.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char* argv[]) {
    try {
        UserInterface ui(argc, argv);
        ui.run();
    } catch (const BasicClientException& e) {
        std::cerr << "Exception: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected Exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
