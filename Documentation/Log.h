#ifndef LOG_H
#define LOG_H

#include <string>

/**
 * @file Log.h
 * @brief Интерфейс класса для логирования ошибок.
 * @author Тришкин В.Д.
 */

/**
 * @brief Класс для записи ошибок в файл лога и вывода их в консоль.
 * 
 * Этот класс предоставляет методы для записи сообщений об ошибках в файл и консоль,
 * а также поддерживает возможность указания критичности ошибки.
 */
class Log {
public:
    /**
     * @brief Путь к файлу логов.
     * 
     * Это статический член класса, который определяет путь к файлу, куда будут записываться логи.
     * Значение по умолчанию — "server_log.txt".
     */
    static std::string logFile; 

    /**
     * @brief Записывает сообщение об ошибке в лог и выводит в консоль.
     * 
     * Этот метод записывает переданное сообщение в файл лога с отметкой времени и выводит его в консоль.
     * Также можно указать, является ли ошибка критической (по умолчанию — нет).
     * 
     * @param message Сообщение, которое будет записано в лог.
     * @param critical Флаг критичности ошибки. По умолчанию — false.
     */
    static void recordError(const std::string& message, bool critical = false);

private:
    /**
     * @brief Получает текущее время в формате "YYYY-MM-DD HH:MM:SS".
     * 
     * Этот метод генерирует строку с текущим временем, которое используется для отметки времени в логе.
     * 
     * @return Строка, представляющая текущее время в формате "YYYY-MM-DD HH:MM:SS".
     */
    static std::string getCurrentTime();
};

#endif // LOG_H

