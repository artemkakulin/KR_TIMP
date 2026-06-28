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

// Тест для чтения конфигурационных данных
TEST(InputOutputManagerConf)
{
    InputOutputManager ioManager(
        "./config/vclient.conf",
        "./input.txt",
        "./output.txt");
    std::array<std::string, 2> conf = ioManager.conf();

    // Проверяем, что конфигурация не пустая
    CHECK(!conf[0].empty());
    CHECK(!conf[1].empty());
}

// Тест для чтения данных
TEST(InputOutputManagerRead)
{
    InputOutputManager ioManager(
        "./config/vclient.conf",
        "./input.txt",
        "./output.txt");
    std::vector<std::vector<int32_t>> data = ioManager.read();

    // Проверяем, что данные не пустые
    CHECK(!data.empty());

    // Проверяем корректность данных, если известно
    CHECK_EQUAL(3, data.size()); // например, если ожидается, что данные содержат 3 строки
}

// Тест для записи данных
TEST(InputOutputManagerWrite)
{
    InputOutputManager ioManager(
        "./config/vclient.conf",
        "./input.txt",
        "./output.txt");
    std::vector<int32_t> data = {1, -2, 3, 4, -5};

    // Пытаемся записать данные
    ioManager.write(data);

    // Здесь можно добавить дополнительные проверки, чтобы убедиться, что данные были успешно записаны
}

// Тест для ошибки открытия конфигурационного файла
TEST(InputOutputManagerConfFileNotFound)
{
    InputOutputManager ioManager(
        "./non_exists_path.conf",
        "./input.txt",
        "./output.txt");
    CHECK_THROW(ioManager.conf(), InputOutputException);
}

// Тест для ошибки отсутствия данных в конфигурационном файле
TEST(InputOutputManagerConfMissingData)
{
    // Подготовка тестового конфигурационного файла без данных
    std::ofstream test_conf("./test.conf");
    test_conf << "username:";
    test_conf.close();

    InputOutputManager ioManager(
        "./test.conf",
        "./input.txt",
        "./output.txt");
    CHECK_THROW(ioManager.conf(), DataDecodeException);

    // Удаление тестового файла после проверки
    std::remove("./test.conf");
}

// Тест для ошибки открытия входного файла
TEST(InputOutputManagerReadFileNotFound)
{
    InputOutputManager ioManager(
        "./config/vclient.conf",
        "./non_exists_file.txt",
        "./output.txt");
    CHECK_THROW(ioManager.read(), InputOutputException);
}

// Тест для ошибки открытия выходного файла
TEST(InputOutputManagerWriteFileNotFound)
{
    InputOutputManager ioManager(
        "./config/vclient.conf",
        "./input.txt",
        "./non_exists_path/non_exists_file.txt");
    CHECK_THROW(ioManager.write({1, 2, 3, 4, 5}), InputOutputException);
}

// Тест для установки соединения
TEST(NetworkManagerConnect)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();

    // Проверка параметров после соединения
    CHECK_EQUAL(std::string("127.0.0.1"), netManager.getAddress());
    CHECK_EQUAL((uint16_t)33333, netManager.getPort());

    netManager.close();
}

// Тест для передачи данных и получения результата
TEST(NetworkManagerAuthAndCalc)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.auth("user", "P@ssW0rd");

    std::vector<std::vector<int32_t>> data = {
        {151, -57, -73, 49, 23},
        {-68, 66, 37, -83, 151}};
    std::vector<int32_t> results;
    results = netManager.calc(data);

    CHECK(!results.empty());

    netManager.close();
}

// Тест для ошибки соединения
TEST(NetworkManagerConnError)
{
    NetworkManager netManager("256.256.256.256", 33333);
    CHECK_THROW(netManager.conn(), NetworkException);
}

// Тест для ошибки аутентификации
TEST(NetworkManagerAuthError)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();
    CHECK_THROW(netManager.auth("invalid_user", "invalid_pass"), AuthException);
    netManager.close();
}

// Тест для закрытия соединения
TEST(NetworkManagerClose)
{
    NetworkManager netManager("127.0.0.1", 33333);
    netManager.conn();
    netManager.close();
}

// Тест для проверки корректной обработки параметров
TEST(UserInterfaceParseArgsCorrect)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-i", "input.bin", "-o", "output.txt", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    UserInterface ui(argc, const_cast<char **>(argv));

    // Проверяем корректность распарсенных параметров
    CHECK_EQUAL(std::string("192.168.0.1"), ui.getAddress());
    CHECK_EQUAL((uint16_t)8080, ui.getPort());
    CHECK_EQUAL(std::string("input.bin"), ui.getInputFilePath());
    CHECK_EQUAL(std::string("output.txt"), ui.getOutputFilePath());
    CHECK_EQUAL(std::string("config.conf"), ui.getConfigFilePath());
}

// Тест для проверки отсутствия обязательного параметра input
TEST(UserInterfaceParseArgsMissingInput)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-o", "output.txt", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
}

// Тест для проверки отсутствия обязательного параметра output
TEST(UserInterfaceParseArgsMissingOutput)
{
    const char *argv[] = {"vclient", "-a", "192.168.0.1", "-p", "8080", "-i", "input.bin", "-c", "config.conf"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
}

// Тест для проверки отсутствия значения для параметра address
TEST(UserInterfaceParseArgsMissingAddressValue)
{
    const char *argv[] = {"vclient", "-a"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
}

// Тест для проверки отсутствия значения для параметра port
TEST(UserInterfaceParseArgsMissingPortValue)
{
    const char *argv[] = {"vclient", "-p"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
}

// Тест для проверки отсутствия значения для параметра config
TEST(UserInterfaceParseArgsMissingConfigValue)
{
    const char *argv[] = {"vclient", "-c"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
}

// Тест для проверки неизвестного параметра
TEST(UserInterfaceParseArgsUnknownParam)
{
    const char *argv[] = {"vclient", "--unknown"};
    int argc = sizeof(argv) / sizeof(argv[0]);

    CHECK_THROW(UserInterface ui(argc, const_cast<char **>(argv)), ArgsDecodeException);
}

/**
 * @brief Класс логгера для вывода детализированной информации о тестах.
 */
class MyTestReporter : public UnitTest::TestReporter
{
public:
    /**
     * @brief Сообщает о начале выполнения теста.
     * @param test Детали теста.
     */
    void ReportTestStart(UnitTest::TestDetails const &test) override
    {
        std::cout << "Test <" << test.testName << "> started:\n";
    }

    /**
     * @brief Сообщает об окончании выполнения теста.
     * @param test Детали теста.
     * @param secondsElapsed Время выполнения теста в секундах.
     */
    void ReportTestFinish(UnitTest::TestDetails const &test, float secondsElapsed) override
    {
        std::cout << "*passed("
                  << secondsElapsed << " seconds)\n"
                  << "================================"
                  << "================================\n";
    }
    void ReportFailure(UnitTest::TestDetails const &test, char const *failure) override
    {
        std::cout << "*failed: "
                  << " (" << failure << ")\n"
                  << "================================"
                  << "================================\n";
    }

    /**
     * @brief Сообщает об общей сводке выполнения тестов.
     * @param totalTestCount Общее количество тестов.
     * @param failedTestCount Количество неудачных тестов.
     * @param failureCount Общее количество сбоев.
     * @param secondsElapsed Общее время выполнения тестов в секундах.
     */
    void ReportSummary(int totalTestCount, int failedTestCount, int failureCount, float secondsElapsed) override
    {
        std::cout << "Summary: "
                  << totalTestCount << " tests, "
                  << failedTestCount << " failed, "
                  << failureCount << " failures, "
                  << secondsElapsed << " seconds\n";
    }
};

/**
 * @brief Главная функция тестирования.
 * @details Инициализирует объект MyTestReporter и запускает тесты с использованием UnitTest++.
 * @param argc Количество аргументов командной строки.
 * @param argv Аргументы командной строки.
 * @return Код завершения программы. 0 - успешное завершение, 1 - ошибка.
 */
int main(int argc, char *argv[])
{
    MyTestReporter reporter;
    UnitTest::TestRunner runner(reporter);
    return runner.RunTestsIf(UnitTest::Test::GetTestList(), nullptr, UnitTest::True(), 0);
}
