#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <string>
#include <vector>
#include <array>
#include "exceptions.h"

/** 
* @file io.h
* @brief Определения классов для управления вводом-выводом.
* @details Этот файл содержит определения классов для работы с файлами ввода-вывода и конфигурационными данными.
* @date 25.06.2026
* @version 1.0
* @authors Какулин А.А.
* @copyright ИБСТ ПГУ
*/

class InputOutputManager {
public:
    InputOutputManager(
        const std::string& config, 
        const std::string& input, 
        const std::string& output
    );

    std::array<std::string, 2> conf();

    std::vector<std::vector<uint64_t>> read();
    void write(const std::vector<uint64_t>& data);

private:
    std::string config;
    std::string input;
    std::string output;
};

#endif // IO_MANAGER_H