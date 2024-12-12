#include "ClientHandler.h"
#include "Log.h"
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

/**
 * @file Server.cpp
 * @brief Реализация сервера для обработки клиентских запросов.
 * @author Тришкин В.Д.
 */

/**
 * @brief Печатает справочную информацию по использованию программы.
 * 
 * Этот метод выводит описание команды для запуска сервера, включая необходимые и опциональные аргументы.
 */
void printHelp() {
    std::cout << "Usage: ./server <log_file> <db_file> [<port>]\n"
              << "\nDescription:\n"
              << "  This server listens on a specified port (default 33333) and handles client requests.\n"
              << "  Arguments:\n"
              << "    <log_file>  Path to the log file.\n"
              << "    <db_file>   Path to the database file.\n"
              << "    <port>      Optional port number (1-65535).\n";
}

/**
 * @brief Основная функция сервера, запускающегося для обработки входящих запросов.
 * 
 * Этот метод настраивает сокет для сервера, принимает клиентские соединения, и передает запросы на обработку.
 * 
 * @param argc Количество аргументов командной строки.
 * @param argv Массив аргументов командной строки.
 * 
 * @return Возвращает 0 при успешном завершении, иначе 1.
 */
int main(int argc, char* argv[]) {
    // Проверка аргументов командной строки для помощи
    if (argc > 1 && (std::strcmp(argv[1], "-h") == 0 || std::strcmp(argv[1], "--help") == 0)) {
        printHelp();
        return 0;
    }

    // Проверка корректности аргументов
    if (argc < 3) {
        std::cerr << "Usage: ./server <log_file> <db_file> [<port>]\n"
                  << "Try './server -h' for more information." << std::endl;
        return 1;
    }

    // Получение параметров
    std::string logFile = argv[1];
    std::string dbFile = argv[2];
    int port = 33333;

    // Определение порта (если указан)
    if (argc > 3) {
        try {
            port = std::stoi(argv[3]);
            if (port < 1 || port > 65535) {
                throw std::out_of_range("Port number must be between 1 and 65535");
            }
        } catch (...) {
            std::cerr << "Error: Invalid port number" << std::endl;
            return 1;
        }
    }

    // Инициализация сокета
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        Log::recordError("Socket creation error", true);
        return -1;
    }

    // Настройка параметров сокета
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        Log::recordError("Setsockopt error", true);
        return -1;
    }

    // Настройка адреса и порта
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Привязка сокета
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        Log::recordError("Bind error", true);
        return -1;
    }

    // Прослушивание порта
    if (listen(server_fd, 3) < 0) {
        Log::recordError("Listen error", true);
        return -1;
    }

    std::cout << "Server is running on port " << port << std::endl;
    while (true) {
        // Принятие входящих соединений
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            Log::recordError("Accept error", true);
            return -1;
        }

        // Обработка запроса клиента
        ClientHandler clientHandler;
        clientHandler.handleRequest(new_socket, dbFile, logFile);
        close(new_socket);
    }

    return 0;
}
