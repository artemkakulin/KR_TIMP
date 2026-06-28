#include "ui.h"
#include <iostream>
#include <cstring>

// Конструктор
UserInterface::UserInterface(int argc, char *argv[])
    : address("127.0.0.1"),
      port(33333),
      config_path("./config/vclient.conf"),
      help_flag(false),
      io_man(nullptr),
      net_man(nullptr)
{
    this->parseArgs(argc, argv);

    if (this->help_flag)
    {
        this->showHelp();
        exit(0);
    }

    // Проверка, что все обязательные параметры заданы
    if (this->input_path.empty() || this->output_path.empty())
    {
        this->showHelp();
        throw ArgsDecodeException(
            "Missing required parameters",
            "UserInterface::UserInterface()");
    }

    this->io_man = new InputOutputManager(
        this->config_path,
        this->input_path,
        this->output_path);
    this->net_man = new NetworkManager(
        this->address,
        this->port);
}

// Деструктор
UserInterface::~UserInterface()
{
    delete this->io_man;
    delete this->net_man;
}
std::string &UserInterface::getAddress()
{
    return this->address;
};
uint16_t &UserInterface::getPort()
{
    return this->port;
};
std::string &UserInterface::getInputFilePath()
{
    return this->input_path;
};
std::string &UserInterface::getOutputFilePath()
{
    return this->output_path;
};
std::string &UserInterface::getConfigFilePath()
{
    return this->config_path;
};
// Метод для разбора аргументов
void UserInterface::parseArgs(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "-h") == 0 || std::strcmp(argv[i], "--help") == 0)
            this->help_flag = true;
        else if (
            std::strcmp(argv[i], "-a") == 0 ||
            std::strcmp(argv[i], "--address") == 0)
        {
            if (i + 1 < argc)
                this->address = argv[++i];
            else
                throw ArgsDecodeException(
                    "Missing value for address parameter",
                    "UserInterface::parseArgs");
        }
        else if (
            std::strcmp(argv[i], "-p") == 0 ||
            std::strcmp(argv[i], "--port") == 0)
        {
            if (i + 1 < argc)
                this->port = std::stoi(argv[++i]);
            else
                throw ArgsDecodeException(
                    "Missing value for port parameter",
                    "UserInterface::parseArgs");
        }
        else if (
            std::strcmp(argv[i], "-i") == 0 ||
            std::strcmp(argv[i], "--input") == 0)
        {
            if (i + 1 < argc)
                this->input_path = argv[++i];
            else
                throw ArgsDecodeException(
                    "Missing value for input parameter",
                    "UserInterface::parseArgs()");
        }
        else if (
            std::strcmp(argv[i], "-o") == 0 ||
            std::strcmp(argv[i], "--output") == 0)
        {
            if (i + 1 < argc)
                this->output_path = argv[++i];
            else
                throw ArgsDecodeException(
                    "Missing value for output parameter",
                    "UserInterface::parseArgs()");
        }
        else if (
            std::strcmp(argv[i], "-c") == 0 ||
            std::strcmp(argv[i], "--config") == 0)
        {
            if (i + 1 < argc)
                this->config_path = argv[++i];
            else
                throw ArgsDecodeException(
                    "Missing value for config parameter",
                    "UserInterface::parseArgs()");
        }
        else
            throw ArgsDecodeException(
                "Unknown parameter: " + std::string(argv[i]),
                "UserInterface::parseArgs()");
    }
}

// Метод для показа справки
void UserInterface::showHelp()
{
    std::cout << "Usage: vclient [options]\n"
              << "Options:\n"
              << "  -h, --help            Show this help message and exit\n"
              << "  -a, --address ADDRESS Server address (default: 127.0.0.1)\n"
              << "  -p, --port PORT       Server port (default: 33333)\n"
              << "  -i, --input PATH      Path to input data file\n"
              << "  -o, --output PATH     Path to output data file\n"
              << "  -c, --config PATH     Path to config file (default: ./config/vclient.conf)\n";
}

// Метод для запуска программы
void UserInterface::run()
{
    // Пример использования методов io_man и net_man
    auto credentials = this->io_man->conf();
    this->net_man->conn();
    this->net_man->auth(credentials[0], credentials[1]);

    auto data = this->io_man->read();
    auto results = this->net_man->calc(data);
    this->io_man->write(results);

    this->net_man->close();
}
