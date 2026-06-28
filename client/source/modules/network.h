#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string>
#include <vector>
#include <cstdint>

/** 
* @file network.h
* @brief Определения классов для управления сетевым взаимодействием.
* @details Этот файл содержит определения классов для управления сетевыми подключениями и передачей данных.
* @date 25.06.2026
* @version 1.0
* @authors Какулин А.А.
* @copyright ИБСТ ПГУ
*/

/** 
* @brief Класс для управления сетевым подключением и взаимодействием.
*/
class NetworkManager
{
public:
    NetworkManager(const std::string &address, uint16_t port);

    std::string &getAddress();
    uint16_t &getPort();

    void conn();
    void auth(const std::string &username, const std::string &password);

    std::vector<uint64_t> calc(const std::vector<std::vector<uint64_t>> &data);

    void close();

private:
    int socket;
    std::string address;
    uint16_t port;
};

#endif // NETWORK_MANAGER_H