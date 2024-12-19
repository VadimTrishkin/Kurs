#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <string>

/**
 * @file DatabaseConnector.h
 * @brief Заголовочный файл для работы с базой данных пользователей.
 * @author Тришкин В.Д.
 */

/**
 * @brief Класс для взаимодействия с базой данных пользователей.
 * 
 * Класс предоставляет методы для аутентификации пользователей, проверяя их логин и хэш пароля.
 * Аутентификация выполняется с использованием соли и хэширования пароля.
 */
class DatabaseConnector {
public:
    /**
     * @brief Проверка аутентификации пользователя.
     * 
     * Метод проверяет, существует ли пользователь в базе данных, и совпадает ли хэш пароля с сохраненным хэшем в базе данных.
     * Хэширование пароля выполняется с использованием MD5 и соли.
     * 
     * @param login Логин пользователя.
     * @param salt Соль, используемая при хэшировании пароля.
     * @param hash Хэш пароля, который передан для проверки.
     * @param dbPath Путь к файлу базы данных.
     * @return Возвращает true, если пользователь найден и хэш пароля совпадает, иначе false.
     */
    bool verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbPath);

private:
    /**
     * @brief Генерация хэша пароля с солью.
     * 
     * Метод генерирует хэш пароля, комбинированного с солью, используя алгоритм MD5.
     * 
     * @param password Пароль пользователя.
     * @param salt Соль, используемая при хэшировании.
     * @return Хэш пароля в виде строки.
     */
    std::string generateHash(const std::string& password, const std::string& salt);

    /**
     * @brief Сравнение двух хэшей.
     * 
     * Метод выполняет регистронезависимое сравнение двух хэшей (хэш сервера и хэш клиента).
     * 
     * @param serverHash Хэш, сохраненный на сервере.
     * @param clientHash Хэш, переданный для проверки.
     * @return Возвращает true, если хэши совпадают, иначе false.
     */
    bool compareHashes(const std::string& serverHash, const std::string& clientHash);
};

#endif // DATABASECONNECTOR_H

