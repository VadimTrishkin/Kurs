#include "Log.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

std::string Log::logFile = "server_log.txt";  // Устанавливается через аргументы при запуске

void Log::recordError(const std::string& message, bool critical) {
    std::ofstream logStream(logFile, std::ios_base::app);
    if (!logStream.is_open()) {
        std::cerr << "Failed to open log file: " << logFile << std::endl;
        return;
    }

    std::string timestamp = getCurrentTime();
    std::string logMessage = "[" + timestamp + "] " + (critical ? "Critical: " : "Warning: ") + message;

    logStream << logMessage << std::endl;
    std::cerr << logMessage << std::endl;
}

std::string Log::getCurrentTime() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    std::ostringstream oss;
    oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
