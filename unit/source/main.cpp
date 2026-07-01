/**
 * @file test_main.cpp
 * @brief Тесты для модулей ввода-вывода, сетевого взаимодействия и пользовательского интерфейса.
 * @details Этот файл содержит тесты для проверки различных компонентов программы, включая ввод-вывод, сетевое взаимодействие и обработку параметров пользовательского интерфейса.
 * @date 25.06.2026
 * @version 1.0
 * @authors Какулин А.А.
 * @copyright ИБСТ ПГУ
 */

#include <UnitTest++/UnitTest++.h>
#include "../../client/source/modules/io.h"
#include "../../client/source/modules/network.h"
#include "../../client/source/modules/ui.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

// Группа тестов для InputOutputManager (I/O)
SUITE(IOTests)
{
    // Test M1.1: Чтение конфигурационных данных (позитивный)
    TEST(InputOutputManagerConf)
    {
        InputOutputManager ioManager(
            "./config/vclient.conf",
            "./input.txt",
            "./output.txt");
        std::array<std::string, 2> conf = ioManager.conf();

        CHECK(!conf[0].empty());
        CHECK(!conf[1].empty());
    }

    // Test M1.2: Чтение данных (векторы) из текстового файла (позитивный)
    TEST(InputOutputManagerRead)
    {
        InputOutputManager ioManager(
            "./config/vclient.conf",
            "./input.txt",
            "./output.txt");
        std::vector<std::vector<uint64_t>> data = ioManager.read();

        CHECK(!data.empty());
        CHECK_EQUAL(3, data.size());
    }

    // Test M1.3: Запись результатов в текстовый файл (позитивный)
    TEST(InputOutputManagerWrite)
    {
        InputOutputManager ioManager(
            "./config/vclient.conf",
            "./input.txt",
            "./output.txt");
        std::vector<uint64_t> data = {1, 2, 3, 4, 5};
        ioManager.write(data);
    }

    // Test M1.4: Ошибка открытия конфигурационного файла (негативный)
    TEST(InputOutputManagerConfFileNotFound)
    {
        InputOutputManager ioManager(
            "./non_exists_path.conf",
            "./input.txt",
            "./output.txt");
        CHECK_THROW(ioManager.conf(), InputOutputException);
    }

    // Test M1.5: Отсутствие данных в конфигурационном файле (негативный)
    TEST(InputOutputManagerConfMissingData)
    {
        std::ofstream test_conf("./test.conf");
        test_conf << "username:";
        test_conf.close();

        InputOutputManager ioManager(
            "./test.conf",
            "./input.txt",
            "./output.txt");
        CHECK_THROW(ioManager.conf(), DataDecodeException);

        std::remove("./test.conf");
    }

    // Test M1.6: Ошибка открытия входного файла (негативный)
    TEST(InputOutputManagerReadFileNotFound)
    {
        InputOutputManager ioManager(
            "./config/vclient.conf",
            "./non_exists_file.txt",
            "./output.txt");
        CHECK_THROW(ioManager.read(), InputOutputException);
    }

    // Test M1.7: Ошибка открытия выходного файла (негативный)
    TEST(InputOutputManagerWriteFileNotFound)
    {
        InputOutputManager ioManager(
            "./config/vclient.conf",
            "./input.txt",
            "./non_exists_path/non_exists_file.txt");
        CHECK_THROW(ioManager.write({1, 2, 3, 4, 5}), InputOutputException);
    }
}

// Группа тестов для NetworkManager (Network)
SUITE(NetworkTests)
{
    // Test M2.1: Установка соединения с сервером (позитивный)
    TEST(NetworkManagerConnect)
    {
        NetworkManager netManager("127.0.0.1", 33333);
        netManager.conn();

        CHECK_EQUAL(std::string("127.0.0.1"), netManager.getAddress());
        CHECK_EQUAL((uint16_t)33333, netManager.getPort());

        netManager.close();
    }

    // Test M2.2: Аутентификация (с солью от сервера) и передача данных (позитивный)
    TEST(NetworkManagerAuthAndCalc)
    {
        NetworkManager netManager("127.0.0.1", 33333);
        netManager.conn();
        netManager.auth("user", "P@ssW0rd");

        std::vector<std::vector<uint64_t>> data = {
            {151, 57, 73, 49, 23},
            {68, 66, 37, 83, 151}};
        std::vector<uint64_t> results = netManager.calc(data);

        CHECK(!results.empty());

        netManager.close();
    }

    // Test M2.3: Ошибка соединения (негативный)
    TEST(NetworkManagerConnError)
    {
        NetworkManager netManager("256.256.256.256", 33333);
        CHECK_THROW(netManager.conn(), NetworkException);
    }

    // Test M2.4: Ошибка аутентификации (негативный)
    TEST(NetworkManagerAuthError)
    {
        NetworkManager netManager("127.0.0.1", 33333);
        netManager.conn();
        CHECK_THROW(netManager.auth("invalid_user", "invalid_pass"), AuthException);
        netManager.close();
    }

    // Test M2.5: Закрытие соединения (позитивный)
    TEST(NetworkManagerClose)
    {
        NetworkManager netManager("127.0.0.1", 33333);
        netManager.conn();
        netManager.close();
    }
}

// Группа тестов для UserInterface (UI)
SUITE(UITests)
{
    // Test M3.1: Парсинг аргументов командной строки (корректные параметры) – позитивный
    TEST(UserInterfaceParseArgsCorrect)
    {
        const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-i", "input.txt", "-o", "output.txt", "-c", "config.conf"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        UserInterface ui(argc, const_cast<char **>(argv));

        CHECK_EQUAL(std::string("192.168.0.1"), ui.getAddress());
        CHECK_EQUAL((uint16_t)8080, ui.getPort());
        CHECK_EQUAL(std::string("input.txt"), ui.getInputFilePath());
        CHECK_EQUAL(std::string("output.txt"), ui.getOutputFilePath());
        CHECK_EQUAL(std::string("config.conf"), ui.getConfigFilePath());
    }

    // Test M3.2: Отсутствие обязательного параметра -i (негативный)
    TEST(UserInterfaceParseArgsMissingInput)
    {
        const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-o", "output.txt", "-c", "config.conf"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
    }

    // Test M3.3: Отсутствие обязательного параметра -o (негативный)
    TEST(UserInterfaceParseArgsMissingOutput)
    {
        const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-i", "input.txt", "-c", "config.conf"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
    }

    // Test M3.4: Отсутствие значения для параметра -a (негативный)
    TEST(UserInterfaceParseArgsMissingAddressValue)
    {
        const char *argv[] = {"vclient", "-a"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
    }

    // Test M3.5: Отсутствие значения для параметра -p (негативный)
    TEST(UserInterfaceParseArgsMissingPortValue)
    {
        const char *argv[] = {"vclient", "-p"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
    }

    // Test M3.6: Отсутствие значения для параметра -c (негативный)
    TEST(UserInterfaceParseArgsMissingConfigValue)
    {
        const char *argv[] = {"vclient", "-c"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
    }

    // Test M3.7: Неизвестный параметр (негативный)
    TEST(UserInterfaceParseArgsUnknownParam)
    {
        const char *argv[] = {"vclient", "--unknown"};
        int argc = sizeof(argv) / sizeof(argv[0]);

        CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
    }
}

/**
 * @brief Класс логгера для вывода детализированной информации о тестах.
 */
class MyTestReporter : public UnitTest::TestReporter
{
public:
    void ReportTestStart(UnitTest::TestDetails const &test) override
    {
        std::cout << "Test <" << test.testName << "> started:\n";
    }

    void ReportTestFinish(UnitTest::TestDetails const &test, float secondsElapsed) override
    {
        std::cout << "*passed(" << secondsElapsed << " seconds)\n"
                  << "================================" << "================================\n";
    }
    void ReportFailure(UnitTest::TestDetails const &test, char const *failure) override
    {
        std::cout << "*failed: " << " (" << failure << ")\n"
                  << "================================" << "================================\n";
    }

    void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) override
    {
        std::cout << "Summary: " << totalTestCount << " tests, "
                  << failedTestCount << " failed, "
                  << failureCount << " failures, "
                  << secondsElapsed << " seconds\n";
    }
};

int main(int argc, char *argv[])
{
    MyTestReporter reporter;
    UnitTest::TestRunner runner(reporter);
    return runner.RunTestsIf(UnitTest::Test::GetTestList(), nullptr, UnitTest::True(), 0);
}