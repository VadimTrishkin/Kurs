#include <UnitTest++/UnitTest++.h>
#include <fstream>
#include <vector>
#include <string>
#include <openssl/md5.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Log.h"
#include "VectorProcessor.h"
#include "DatabaseConnector.h"
#include "ClientHandler.h"

// Тесты для Log
// Тест вызова сообщения о некритической ошибке
TEST(Log_Warning_Message) {
    Log::logFile = "test_log.txt";
    Log::recordError("Test warning message", false);

    std::ifstream logFile("test_log.txt");
    std::string line;
    bool found = false;

    while (std::getline(logFile, line)) {
        if (line.find("Test warning message") != std::string::npos &&
            line.find("Warning: ") != std::string::npos) {
            found = true;
            break;
        }
    }

    CHECK(found);
}

// Тест вызова сообщения о критической ошибке
TEST(Log_Critical_Message) {
    Log::logFile = "test_log.txt";
    Log::recordError("Test critical message", true);

    std::ifstream logFile("test_log.txt");
    std::string line;
    bool found = false;

    while (std::getline(logFile, line)) {
        if (line.find("Test critical message") != std::string::npos &&
            line.find("Critical: ") != std::string::npos) {
            found = true;
            break;
        }
    }

    CHECK(found);
}


// Тесты для VectorProcessor
TEST(VectorProcessor_Valid_Vector) {
    VectorProcessor processor;
    std::vector<int16_t> vec = {1, 2, 3};
    int16_t result = processor.computeProduct(vec);

    CHECK_EQUAL(6, result);  // 1 * 2 * 3 = 6
}

TEST(VectorProcessor_Vector_Overflow) {
    VectorProcessor processor;
    std::vector<int16_t> vec = {32767, 2};  // Ожидается переполнение
    int16_t result = processor.computeProduct(vec);

    CHECK_EQUAL(32767, result);  // Переполнение, возвращаем max int16_t
}

TEST(VectorProcessor_Vector_Underflow) {
    VectorProcessor processor;
    std::vector<int16_t> vec = {-32768, 2};  // Ожидается переполнение в отрицательную сторону
    int16_t result = processor.computeProduct(vec);

    CHECK_EQUAL(-32768, result);  // Переполнение, возвращаем min int16_t
}

// Тесты для DatabaseConnector
TEST(DatabaseConnector_Valid_User) {
    DatabaseConnector dbConn;
    std::string login = "user";
    std::string salt = "1111111111111111";
    std::string hash = dbConn.generateHash("password_hash", salt);  
    bool result = dbConn.verifyUser(login, salt, hash, "test_db.txt");

    CHECK(result);
}

TEST(DatabaseConnector_Invalid_User) {
    DatabaseConnector dbConn;
    std::string login = "invalid_user";
    std::string salt = "1111111111111111";
    std::string hash = dbConn.generateHash("password_hash", salt);  // В этом тесте предполагаем неправильный логин
    bool result = dbConn.verifyUser(login, salt, hash, "test_db.txt");

    CHECK(!result);
}

TEST(DatabaseConnector_Incorrect_Hash) {
    DatabaseConnector dbConn;
    std::string login = "user";
    std::string salt = "1111111111111111";
    std::string incorrectHash = "incorrect_hash_value"; // Неправильный хэш
    bool result = dbConn.verifyUser(login, salt, incorrectHash, "test_db.txt");

    CHECK(!result);
}

TEST(DatabaseConnector_Missing_File) {
    DatabaseConnector dbConn;
    std::string login = "user";
    std::string salt = "1111111111111111";
    std::string hash = dbConn.generateHash("password_hash", salt);

    bool result = dbConn.verifyUser(login, salt, hash, "missing_db.txt");

    CHECK(!result);  // База данных не существует
}

// Тесты для ClientHandler
TEST(ClientHandler_Valid_Request) {
    ClientHandler clientHandler;
    std::vector<int16_t> vec = {1, 2, 3};
    int16_t result = clientHandler.processVector(vec);

    CHECK_EQUAL(6, result);  // 1 * 2 * 3 = 6
}

TEST(ClientHandler_Invalid_User) {
    DatabaseConnector dbConn;
    bool result = dbConn.verifyUser("user", "1111111111111111", "incorrect_hash", "test_db.txt");

    CHECK(!result);  // Неверный пользователь
}

TEST(ClientHandler_Successful_Authentication) {
    DatabaseConnector dbConn;
    std::string login = "user";
    std::string salt = "1111111111111111";
    std::string hash = dbConn.generateHash("password_hash", salt);  // В этом тесте предполагаем правильный хеш
    bool result = dbConn.verifyUser(login, salt, hash, "test_db.txt");

    CHECK(result);  // Ожидаем успешную аутентификацию
}

// Генерация данных для DatabaseConnector тестов (создаем фиктивную базу данных)
void createTestDB() {
    std::ofstream dbFile("test_db.txt");
    dbFile << "user:password_hash\n";
    dbFile.close();
}

int main() {
    createTestDB();  // Создаем тестовую базу данных

    // Запуск всех тестов
    return UnitTest::RunAllTests();
}
