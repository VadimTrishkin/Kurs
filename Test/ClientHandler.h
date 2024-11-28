#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <string>
#include <vector>
#include "Log.h"
#include "DatabaseConnector.h"
#include "VectorProcessor.h"

class ClientHandler {
public:
    bool verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbFile) {
        DatabaseConnector dbConn;
        return dbConn.verifyUser(login, salt, hash, dbFile);
    }

    int16_t processVector(const std::vector<int16_t>& vec) {
        VectorProcessor processor;
        return processor.computeProduct(vec);
    }
};

#endif // CLIENTHANDLER_H
