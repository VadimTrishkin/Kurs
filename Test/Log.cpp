#include "Log.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

// Инициализация статической переменной logFile
std::string Log::logFile = "test_log.txt";  // Путь к файлу логов (создается в текущей директории)

// Метод для записи ошибок в лог
void Log::recordError(const std::string& message, bool critical) {
    // Открытие файла для дозаписи
    std::ofstream logStream(logFile, std::ios_base::app);
    
    // Проверка успешности открытия файла
    if (!logStream.is_open()) {
        std::cerr << "Failed to open log file: " << logFile << std::endl;
        return;
    }

    // Формирование строки с меткой времени и сообщением
    std::string timestamp = getCurrentTime();
    std::string logMessage = "[" + timestamp + "] " + (critical ? "Critical: " : "Warning: ") + message;
    
    // Запись в файл
    logStream << logMessage << std::endl;
    // Также выводим в консоль
    std::cerr << logMessage << std::endl;
}

// Вспомогательный метод для получения текущего времени в нужном формате
std::string Log::getCurrentTime() {
    std::time_t now = std::time(nullptr);               // Получение текущего времени
    std::tm* localTime = std::localtime(&now);          // Преобразование в локальное время

    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S"); // Форматирование времени
    return oss.str();
}
