#ifndef UI_H
#define UI_H

#include "io.h"
#include "network.h"
#include "exceptions.h"
#include <string>
#include <vector>

/** 
* @file ui.h
* @brief Определения классов для пользовательского интерфейса.
* @details Этот файл содержит определения классов для обработки пользовательского ввода и управления интерфейсом.
* @date 25.06.2026
* @version 1.0
* @authors Какулин А.А.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Класс для управления пользовательским интерфейсом.
*/
class UserInterface
{
public:
    /**
    * @brief Конструктор класса UserInterface.
    * @param argc Количество аргументов командной строки.
    * @param argv Аргументы командной строки.
    * @throw ArgsDecodeException Если отсутствуют обязательные параметры или их значения.
    */
    UserInterface(int argc, char *argv[]);

    /**
    * @brief Деструктор класса UserInterface.
    */
    ~UserInterface();

    /**
    * @brief Метод для получения адреса.
    * @return Адрес.
    */
    std::string &getAddress();

    /**
    * @brief Метод для получения порта.
    * @return Порт.
    */
    uint16_t &getPort();

    /**
    * @brief Метод для получения пути к входному файлу.
    * @return Путь к входному файлу.
    */
    std::string &getInputFilePath();

    /**
    * @brief Метод для получения пути к выходному файлу.
    * @return Путь к выходному файлу.
    */
    std::string &getOutputFilePath();

    /**
   * @brief Метод для получения пути к конфигурационному файлу.
    * @return Путь к конфигурационному файлу.
    */
    std::string &getConfigFilePath();

    /**
    * @brief Метод для запуска интерфейса.
    */
    void run();

private:
    std::string address; ///< Адрес сервера.
    uint16_t port; ///< Порт сервера.
    std::string input_path; ///< Путь к входному файлу.
    std::string output_path; ///< Путь к выходному файлу.
    std::string config_path; ///< Путь к файлу конфигурации.

    InputOutputManager *io_man; ///< Менеджер ввода-вывода.
    NetworkManager *net_man; ///< Менеджер сетевого взаимодействия.

    bool help_flag; ///< Флаг для отображения справки.

    /**
    * @brief Вспомогательный метод для разбора аргументов командной строки.
    * @param argc Количество аргументов командной строки.
    * @param argv Аргументы командной строки.
    * @throw ArgsDecodeException Если отсутствуют значения для параметров или найден неизвестный параметр.
    */
    void parseArgs(int argc, char *argv[]);

    /**
    * @brief Метод для отображения справки.
    */
    void showHelp();
};

#endif // UI_H
