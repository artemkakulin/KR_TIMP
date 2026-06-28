#include "exceptions.h"

// Реализация конструктора BasicClientException
BasicClientException::BasicClientException(
    const std::string &name,
    const std::string &message,
    const std::string &func)
    : name(name), message(message), func(func) {}

const char *BasicClientException::what() const noexcept
{
    message = name + " in " + func + "\nMessage: " + message + ".";
    return message.c_str();
}

InputOutputException::InputOutputException(const std::string &message, const std::string &func)
    : BasicClientException("InputOutputException", message, func) {}

ArgsDecodeException::ArgsDecodeException(const std::string &message, const std::string &func)
    : BasicClientException("ArgsDecodeException", message, func) {}

DataDecodeException::DataDecodeException(const std::string &message, const std::string &func)
    : BasicClientException("DataDecodeException", message, func) {}

AuthException::AuthException(const std::string &message, const std::string &func)
    : BasicClientException("AuthException", message, func) {}

NetworkException::NetworkException(const std::string &message, const std::string &func)
    : BasicClientException("NetworkException", message, func) {}
