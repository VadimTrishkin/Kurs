#ifndef LOG_H
#define LOG_H

#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

class Log {
public:
    static std::string logFile;  // Путь к файлу логов

    // Статический метод для записи ошибок в лог
    static void recordError(const std::string& message, bool critical = false) {
        std::ofstream logStream(logFile, std::ios_base::app);  // Открытие файла для дозаписи
        if (!logStream.is_open()) {
            std::cerr << "Failed to open log file: " << logFile << std::endl;
            return;
        }

        std::string timestamp = getCurrentTime();
        std::string logMessage = "[" + timestamp + "] " + (critical ? "Critical: " : "Warning: ") + message;
        logStream << logMessage << std::endl;
        std::cerr << logMessage << std::endl;
    }

private:
    // Вспомогательный метод для получения текущего времени
    static std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

// Инициализация статической переменной logFile
std::string Log::logFile = "test_log.txt";  // Файл будет создаваться в текущей рабочей директории

#endif // LOG_H
