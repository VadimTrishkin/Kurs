#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <string>

class DatabaseConnector {
public:
    bool verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbPath);

private:
    std::string generateHash(const std::string& password, const std::string& salt);
    bool compareHashes(const std::string& serverHash, const std::string& clientHash);
};

#endif // DATABASECONNECTOR_H
