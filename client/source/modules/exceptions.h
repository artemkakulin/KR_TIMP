#ifndef ERRORS_H
#define ERRORS_H

#include <exception>
#include <string>

/** 
* @file exceptions.h
* @brief Определения классов исключений для клиентской части.
* @details Этот файл содержит определения классов для обработки различных исключений, возникающих в клиентской части программы.
* @date 25.06.2026
* @version 1.0
* @authors Какулин А.А.
* @copyright ИБСТ ПГУ
*/
/** 
* @brief Базовый класс для клиентских ошибок.
*/
class BasicClientException : public std::exception
{
public:
    /**
    * @brief Конструктор BasicClientException.
    * @param name Имя исключения.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    BasicClientException(const std::string &name, const std::string &message, const std::string &func);

    /**
    * @brief Метод для получения сообщения об ошибке.
    * @return Сообщение об ошибке.
    */
    const char *what() const noexcept override;

protected:
    std::string name;  ///< Имя исключения.
    std::string func;  ///< Имя функции, в которой возникла ошибка.
    mutable std::string message;  ///< Сообщение об ошибке.
};

/** 
* @brief Исключение для ошибок ввода-вывода.
*/
class InputOutputException : public BasicClientException
{
public:
    /**
    * @brief Конструктор InputOutputException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    InputOutputException(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для ошибок декодирования аргументов.
*/
class ArgsDecodeException : public BasicClientException
{
public:
    /**
    * @brief Конструктор ArgsDecodeException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    ArgsDecodeException(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для ошибок декодирования данных.
*/
class DataDecodeException : public BasicClientException
{
public:
    /**
    * @brief Конструктор DataDecodeException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    DataDecodeException(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для ошибок аутентификации.
*/
class AuthException : public BasicClientException
{
public:
    /**
    * @brief Конструктор AuthException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    AuthException(const std::string &message, const std::string &func);
};

/** 
* @brief Исключение для сетевых ошибок.
*/
class NetworkException : public BasicClientException
{
public:
    /**
    * @brief Конструктор NetworkException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникла ошибка.
    */
    NetworkException(const std::string &message, const std::string &func);
};

#endif // ERRORS_H
