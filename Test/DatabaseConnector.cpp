#include "DatabaseConnector.h"
#include <fstream>
#include <sstream>
#include <openssl/md5.h>
#include <iomanip>

bool DatabaseConnector::verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbPath) {
    std::ifstream dbFile(dbPath);
    if (!dbFile.is_open()) {
        return false;
    }

    std::string line;
    while (std::getline(dbFile, line)) {
        size_t colonPos = line.find(":");
        if (colonPos == std::string::npos) continue;

        std::string storedLogin = line.substr(0, colonPos);
        std::string storedPassword = line.substr(colonPos + 1);

        if (storedLogin == login) {
            std::string hashedPassword = generateHash(storedPassword, salt);
            return compareHashes(hashedPassword, hash);
        }
    }
    return false;
}

std::string DatabaseConnector::generateHash(const std::string& password, const std::string& salt) {
    std::string data = salt + password;
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), result);

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
    }
    return ss.str();
}

bool DatabaseConnector::compareHashes(const std::string& serverHash, const std::string& clientHash) {
    return std::equal(serverHash.begin(), serverHash.end(), clientHash.begin(), clientHash.end(),
        [](unsigned char a, unsigned char b) { return tolower(a) == tolower(b); });
}
