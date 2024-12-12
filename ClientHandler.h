#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <string>
#include <vector>
#include "Log.h"
#include "DatabaseConnector.h"
#include "VectorProcessor.h"
#include <iostream>
#include <cstring>

class ClientHandler {
public:
    // Метод для обработки запроса от клиента
    void handleRequest(int socket, const std::string& dbFile, const std::string& logFile);

private:
    // Метод для проверки типа данных вектора (должны быть int16_t)
    bool validateVectorType(const std::vector<int16_t>& vec);
};

#endif // CLIENTHANDLER_H
