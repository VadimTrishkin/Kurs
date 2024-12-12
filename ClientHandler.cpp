#include "ClientHandler.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <sys/socket.h>   
#include <unistd.h>  

void ClientHandler::handleRequest(int socket, const std::string& dbFile, const std::string& logFile) {
    char buffer[512] = {0};
    if (recv(socket, buffer, sizeof(buffer) - 1, 0) <= 0) {
        Log::recordError("Failed to receive initial data");
        return;
    }
    buffer[sizeof(buffer) - 1] = '\0';

    std::string requestData(buffer);
    std::cout << "Received data: " << requestData << std::endl;

    size_t saltStart = requestData.size() - 48;
    std::string login = requestData.substr(0, saltStart);
    std::string salt = requestData.substr(saltStart, 16);
    std::string hash = requestData.substr(saltStart + 16);

    std::cout << "Login: " << login << std::endl;
    std::cout << "Salt: " << salt << std::endl;
    std::cout << "Hash: " << hash << std::endl;

    DatabaseConnector dbConn;
    if (dbConn.verifyUser(login, salt, hash, dbFile)) {
        send(socket, "OK", 2, 0);
        Log::recordError("User " + login + " authenticated successfully.");
    } else {
        Log::recordError("Authentication failed for " + login);
        send(socket, "ERR", 3, 0);
        close(socket);
    }

    // Обработка векторов
    int32_t vector_count;
    if (recv(socket, &vector_count, sizeof(vector_count), 0) <= 0) {
        Log::recordError("Failed to receive vector count");
        return;
    }
    std::cout << "Received vector count: " << vector_count << std::endl;

    VectorProcessor processor;

    for (int i = 0; i < vector_count; ++i) {
        int32_t vector_size;
        if (recv(socket, &vector_size, sizeof(vector_size), 0) <= 0) {
            Log::recordError("Failed to receive vector size");
            return;
        }
        std::cout << "Received vector size: " << vector_size << std::endl;

        // Проверка на слишком большой вектор
        const size_t MAX_VECTOR_SIZE = 10000;  // Ограничение на размер вектора
        if (vector_size > MAX_VECTOR_SIZE) {
            Log::recordError("Received vector is too large: " + std::to_string(vector_size));
            send(socket, "ERR_TOO_LARGE_VECTOR", 20, 0);
            return;
        }

        std::vector<int16_t> vec(vector_size);
        if (recv(socket, vec.data(), vector_size * sizeof(int16_t), 0) <= 0) {
            Log::recordError("Failed to receive vector values");
            return;
        }

        // Проверка на пустой вектор
        if (vec.empty()) {
            Log::recordError("Received an empty vector");
            send(socket, "ERR_EMPTY_VECTOR", 16, 0);
            return;
        }

        std::cout << "Received vector values: ";
        for (auto val : vec) {
            std::cout << val << " ";
        }
        std::cout << std::endl;

        int16_t result = processor.computeProduct(vec);
        send(socket, &result, sizeof(result), 0);  // Отправка результата
        std::cout << "Sent result: " << result << std::endl;
    }
}