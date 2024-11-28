#include "ClientHandler.h"
#include "Log.h"
#include "DatabaseConnector.h"
#include "VectorProcessor.h"

bool ClientHandler::verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbFile) {
    DatabaseConnector dbConn;
    return dbConn.verifyUser(login, salt, hash, dbFile);
}

int16_t ClientHandler::processVector(const std::vector<int16_t>& vec) {
    VectorProcessor processor;
    return processor.computeProduct(vec);
}
