#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <string>
#include <fstream>
#include <sstream>
#include <openssl/md5.h>
#include <iostream>
#include <algorithm>

class DatabaseConnector {
public:
    bool verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbPath) {
        std::ifstream dbFile(dbPath);
        if (!dbFile.is_open()) {
            return false;  // Ошибка открытия файла базы данных
        }

        std::string line;
        while (std::getline(dbFile, line)) {
            size_t colonPos = line.find(":");
            if (colonPos == std::string::npos) {
                continue;  // Пропускаем строки без двоеточий
            }

            std::string storedLogin = line.substr(0, colonPos);
            std::string storedPassword = line.substr(colonPos + 1);

            if (storedLogin == login) {
                std::string hashedPassword = generateHash(storedPassword, salt);
                return compareHashes(hashedPassword, hash);
            }
        }
        return false;  // Пользователь не найден
    }

    std::string generateHash(const std::string& password, const std::string& salt) {
        std::string data = salt + password;
        unsigned char result[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), result);

        std::stringstream ss;
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
        }
        return ss.str();
    }

private:
    bool compareHashes(const std::string& serverHash, const std::string& clientHash) {
        if (serverHash.size() != clientHash.size()) {
            return false;  // Если длина хешей не совпадает, они точно разные
        }

        // Сравниваем символы, игнорируя регистр
        for (size_t i = 0; i < serverHash.size(); ++i) {
            if (std::tolower(static_cast<unsigned char>(serverHash[i])) != std::tolower(static_cast<unsigned char>(clientHash[i]))) {
                return false;
            }
        }

        return true;  // Все символы совпали, хеши одинаковые
    }
};

#endif // DATABASECONNECTOR_H
