#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <string>
#include <vector>
#include "Log.h"
#include "DatabaseConnector.h"
#include "VectorProcessor.h"
#include <iostream>
#include <cstring>

/**
 * @file ClientHandler.h
 * @brief Заголовочный файл для класса ClientHandler, который обрабатывает запросы от клиента.
 * @author Тришкин В.Д.
 */

/**
 * @brief Класс для обработки запросов от клиента.
 * 
 * Этот класс предназначен для обработки запросов от клиентов через сокеты, включая аутентификацию, 
 * обработку векторов и отправку результатов обратно клиенту.
 * 
 * Основные функции класса включают:
 * - аутентификацию пользователя,
 * - обработку векторов, включая проверку на правильность данных,
 * - отправку результатов клиенту.
 */
class ClientHandler {
public:
    /**
     * @brief Обработка запроса от клиента.
     * 
     * Этот метод выполняет все необходимые шаги для обработки запроса от клиента, включая аутентификацию
     * пользователя, прием и обработку векторов, а также отправку результатов обратно клиенту.
     * 
     * @param socket Дескриптор сокета для связи с клиентом.
     * @param dbFile Путь к файлу базы данных для аутентификации пользователя.
     * @param logFile Путь к файлу для записи логов.
     */
    void handleRequest(int socket, const std::string& dbFile, const std::string& logFile);

private:
    /**
     * @brief Проверка типа данных вектора.
     * 
     * Этот метод проверяет, что элементы вектора имеют правильный тип данных (int16_t).
     * Вектор должен содержать только элементы типа int16_t.
     * 
     * @param vec Вектор для проверки.
     * @return Возвращает true, если вектор содержит элементы типа int16_t, иначе false.
     */
    bool validateVectorType(const std::vector<int16_t>& vec);
};

#endif // CLIENTHANDLER_H

