#include "DatabaseConnector.h"
#include <fstream>
#include <sstream>
#include <openssl/md5.h>
#include <iomanip>
#include <algorithm>

/**
 * @file DatabaseConnector.cpp
 * @brief Реализация класса DatabaseConnector для работы с базой данных.
 * @author Тришкин В.Д.
 * 
 * Этот файл содержит реализацию методов для работы с базой данных пользователей, включая проверку пользователя по логину, соли и хешу пароля.
 */

/**
 * @brief Проверяет пользователя в базе данных.
 * 
 * Метод проверяет логин и хеш пароля с солью в базе данных. Сначала из базы данных извлекаются логин и хеш пароля, после чего вычисляется хеш пароля клиента с использованием соли и сравнивается с хешом из базы данных.
 * 
 * @param login Логин пользователя для проверки.
 * @param salt Соль, используемая для хеширования пароля.
 * @param hash Хеш пароля, полученный от клиента.
 * @param dbPath Путь к файлу базы данных, содержащей логины и пароли.
 * 
 * @return Возвращает true, если логин и хеш пароля совпадают с данными в базе данных, иначе возвращает false.
 */


bool DatabaseConnector::verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbPath) {
    std::ifstream dbFile(dbPath);
    if (!dbFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(dbFile, line)) {
        size_t colonPos = line.find(":");
        if (colonPos == std::string::npos) {
            continue;
        }

        std::string storedLogin = line.substr(0, colonPos);
        std::string storedPassword = line.substr(colonPos + 1);

        if (storedLogin == login) {
            std::string hashedPassword = generateHash(storedPassword, salt);
            return compareHashes(hashedPassword, hash);
        }
    }
    return false;
}

/**
 * @brief Генерирует хеш пароля с использованием соли.
 * 
 * Метод генерирует MD5 хеш пароля, используя соль, которая добавляется к паролю перед хешированием. Этот хеш затем сравнивается с хешем, переданным от клиента.
 * 
 * @param password Пароль, который нужно захешировать.
 * @param salt Соль, которая добавляется к паролю перед хешированием.
 * 
 * @return Возвращает строку, содержащую хеш пароля с солью в формате шестнадцатеричного числа.
 */

std::string DatabaseConnector::generateHash(const std::string& password, const std::string& salt) {
    std::string data = salt + password;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), result);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }
    return ss.str();
}

/**
 * @brief Сравнивает два хеша (серверный и клиентский).
 * 
 * Метод сравнивает хеш пароля, полученный на сервере, с хешем, переданным от клиента. Сравнение выполняется без учета регистра.
 * 
 * @param serverHash Хеш, вычисленный сервером.
 * @param clientHash Хеш, полученный от клиента.
 * 
 * @return Возвращает true, если хеши совпадают (без учета регистра), иначе возвращает false.
 */

bool DatabaseConnector::compareHashes(const std::string& serverHash, const std::string& clientHash) {
    return std::equal(serverHash.begin(), serverHash.end(), clientHash.begin(), clientHash.end(),
        [](unsigned char a, unsigned char b) { return tolower(a) == tolower(b); });
}
