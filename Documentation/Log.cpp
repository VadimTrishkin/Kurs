#include "Log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

/**
 * @file Log.cpp
 * @brief Реализация логирования сообщений об ошибках.
 * @author Тришкин В.Д.
 */

/**
 * @brief Класс для записи логов в файл и вывода ошибок в консоль.
 * 
 * Класс позволяет записывать сообщения об ошибках в файл и выводить их в консоль.
 * Также поддерживается отметка времени для каждого сообщения.
 */
std::string Log::logFile = "server_log.txt";  ///< Путь к файлу логов (по умолчанию "server_log.txt")

/**
 * @brief Записывает сообщение об ошибке в лог-файл и выводит в консоль.
 * 
 * Метод записывает сообщение в файл лога, а также выводит его в консоль. При этом добавляется отметка времени и указание на критичность сообщения.
 * 
 * @param message Сообщение, которое необходимо записать в лог.
 * @param critical Флаг, указывающий на критичность ошибки. Если true, будет указано, что ошибка критична.
 */
void Log::recordError(const std::string& message, bool critical) {
    std::ofstream logStream(logFile, std::ios_base::app);  ///< Открытие файла для записи в конец
    if (!logStream.is_open()) {  ///< Проверка на успешное открытие файла
        std::cerr << "Failed to open log file: " << logFile << std::endl;
        return;
    }

    std::string timestamp = getCurrentTime();  ///< Получаем текущую дату и время
    std::string logMessage = "[" + timestamp + "] " + (critical ? "Critical: " : "Warning: ") + message;

    logStream << logMessage << std::endl;  ///< Записываем сообщение в лог
    std::cerr << logMessage << std::endl;  ///< Выводим сообщение на консоль
}

/**
 * @brief Получает текущую дату и время в формате "YYYY-MM-DD HH:MM:SS".
 * 
 * Метод возвращает текущие дату и время в строковом формате, используя локальное время.
 * 
 * @return Строка с текущей датой и временем в формате "YYYY-MM-DD HH:MM:SS".
 */
std::string Log::getCurrentTime() {
    std::time_t now = std::time(nullptr);  ///< Получаем текущее время
    std::tm* localTime = std::localtime(&now);  ///< Преобразуем в локальное время

    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");  ///< Форматируем строку
    return oss.str();  ///< Возвращаем строку с текущим временем
}
