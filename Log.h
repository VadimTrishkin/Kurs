#ifndef LOG_H
#define LOG_H

#include <string>

class Log {
public:
    static std::string logFile;  // Путь к файлу логов

    static void recordError(const std::string& message, bool critical = false);

private:
    static std::string getCurrentTime();
};

#endif // LOG_H
