#include <cstring>
#include <unistd.h>
#include <iostream>

#include "network.h"
#include "exceptions.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>      // для SHA-1
#include <cryptopp/osrng.h>

NetworkManager::NetworkManager(const std::string &address, uint16_t port)
    : address(address), port(port), socket(-1) {}

std::string &NetworkManager::getAddress() { return this->address; }
uint16_t &NetworkManager::getPort() { return this->port; }

void NetworkManager::conn() {
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket < 0)
        throw NetworkException("Failed to create socket", "NetworkManager.conn()");

    struct sockaddr_in server_addr;
    std::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);

    if (inet_pton(AF_INET, this->address.c_str(), &server_addr.sin_addr) <= 0)
        throw NetworkException("Invalid address/ Address not supported", "NetworkManager.conn()");

    if (connect(this->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        throw NetworkException("Connection failed", "NetworkManager.conn()");
}

void NetworkManager::auth(const std::string &login, const std::string &password) {
    // Отправка логина
    if (send(this->socket, login.c_str(), login.size(), 0) < 0)
        throw AuthException("Failed to send login", "NetworkManager.auth()");

    // Получение соли (16 символов)
    char salt[17];
    int salt_length = recv(this->socket, salt, sizeof(salt) - 1, 0);
    if (salt_length < 0)
        throw AuthException("Failed to receive salt", "NetworkManager.auth()");
    salt[salt_length] = '\0';

    // ИЗМЕНЕНО: SHA256 -> SHA1
    CryptoPP::SHA1 hash_func;
    std::string hash_hex;
    CryptoPP::StringSource(
        std::string(salt) + password,
        true,
        new CryptoPP::HashFilter(
            hash_func,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(hash_hex),
                true // заглавные буквы
            )
        )
    );

    if (send(this->socket, hash_hex.c_str(), hash_hex.size(), 0) < 0)
        throw AuthException("Failed to send hash", "NetworkManager.auth()");

    char response[1024];
    int response_length = recv(this->socket, response, sizeof(response) - 1, 0);
    if (response_length < 0)
        throw AuthException("Failed to receive auth response", "NetworkManager.auth()");
    response[response_length] = '\0';

    if (std::string(response) != "OK")
        throw AuthException("Authentication failed", "NetworkManager.auth()");
}

std::vector<uint64_t> NetworkManager::calc(const std::vector<std::vector<uint64_t>> &data) {
    uint32_t num_vectors = data.size();
    if (send(this->socket, &num_vectors, sizeof(num_vectors), 0) < 0)
        throw NetworkException("Failed to send number of vectors", "NetworkManager.calc()");

    for (const auto &vec : data) {
        uint32_t vec_size = vec.size();
        if (send(this->socket, &vec_size, sizeof(vec_size), 0) < 0)
            throw NetworkException("Failed to send vector size", "NetworkManager.calc()");
        if (send(this->socket, vec.data(), vec_size * sizeof(uint64_t), 0) < 0)
            throw NetworkException("Failed to send vector data", "NetworkManager.calc()");
    }

    std::vector<uint64_t> results(num_vectors);
    for (uint32_t i = 0; i < num_vectors; ++i) {
        if (recv(this->socket, &results[i], sizeof(uint64_t), 0) < 0)
            throw NetworkException("Failed to receive result", "NetworkManager.calc()");
    }

    // Логирование (можно оставить как есть, вывод чисел)
    std::cout << "Log: \"NetworkManager.calc()\"\n";
    std::cout << "Results: {";
    for (const auto &val : results) {
        std::cout << val << ", ";
    }
    if (!results.empty()) std::cout << "\b\b";
    std::cout << "}\n";

    return results;
}

void NetworkManager::close() {
    if (this->socket >= 0) {
        ::close(this->socket);
        this->socket = -1;
    }
}