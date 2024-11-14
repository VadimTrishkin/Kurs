#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <openssl/md5.h>
#include <arpa/inet.h>
#include <iomanip>
#include <ctime>
#include <algorithm>
#include <stdexcept>
#include <cstring>
#include <limits>

// Класс для записи ошибок

class Log {
public:
    static std::string logFile;  // Путь к файлу логов

    static void recordError(const std::string& message, bool critical = false) {
        // Открываем файл логов в режиме добавления (или создаем его, если не существует)
        std::ofstream logStream(logFile, std::ios_base::app);
        if (!logStream.is_open()) {
            std::cerr << "Failed to open log file: " << logFile << std::endl;
            return;
        }

        // Получаем текущие дату и время
        std::string timestamp = getCurrentTime();

        // Формируем сообщение лога
        std::string logMessage = "[" + timestamp + "] " + (critical ? "Critical: " : "Warning: ") + message;

        // Записываем сообщение в файл логов
        logStream << logMessage << std::endl;

        // Также выводим сообщение в консоль
        std::cerr << logMessage << std::endl;
    }

private:
    static std::string getCurrentTime() {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);

        std::ostringstream oss;
        oss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }
};

std::string Log::logFile = "server_log.txt";  // Устанавливается через аргументы при запуске


// Класс для работы с векторами
class VectorProcessor {
public:
    int16_t computeProduct(const std::vector<int16_t>& vector) {
        int32_t product = 1;

        for (const auto& elem : vector) {
            product *= elem;

            if (product > std::numeric_limits<int16_t>::max()) {
                return std::numeric_limits<int16_t>::max();
            }

            if (product < std::numeric_limits<int16_t>::min()) {
                return std::numeric_limits<int16_t>::min();
            }
        }
        return static_cast<int16_t>(product);
    }
};

// Класс для работы с базой данных
class DatabaseConnector {
public:
    bool verifyUser(const std::string& login, const std::string& salt, const std::string& hash, const std::string& dbPath) {
        std::ifstream dbFile(dbPath);
        if (!dbFile.is_open()) {
            Log::recordError("Failed to open database", true);
            return false;
        }

        std::string line;
        while (std::getline(dbFile, line)) {
            size_t colonPos = line.find(":");
            if (colonPos == std::string::npos) {
                continue;  // Пропускаем строки без двоеточий
            }

            std::string storedLogin = line.substr(0, colonPos);
            std::string storedPassword = line.substr(colonPos + 1);

            if (storedLogin == login) {
                std::string hashedPassword = generateHash(storedPassword, salt);
                return compareHashes(hashedPassword, hash);
            }
        }
        return false;
    }

private:
    std::string generateHash(const std::string& password, const std::string& salt) {
        std::string data = salt + password;
        unsigned char result[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(data.c_str()), data.size(), result);

        std::stringstream ss;
        for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(result[i]);
        }
        return ss.str();
    }

    bool compareHashes(const std::string& serverHash, const std::string& clientHash) {
        return std::equal(serverHash.begin(), serverHash.end(), clientHash.begin(), clientHash.end(),
            [](unsigned char a, unsigned char b) { return tolower(a) == tolower(b); });
    }
};

// Класс для взаимодействия с клиентом
class ClientHandler {
public:
    void handleRequest(int socket, const std::string& dbFile, const std::string& logFile) {
        char buffer[512] = {0};  // Увеличиваем размер буфера
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

        // Печать состояния на каждом шаге работы с векторами
        int32_t vector_count;
        if (recv(socket, &vector_count, sizeof(vector_count), 0) <= 0) {
            Log::recordError("Failed to receive vector count");
            return;
        }
        std::cout << "Received vector count: " << vector_count << std::endl;

        VectorProcessor processor; // Инициализация processor здесь

        for (int i = 0; i < vector_count; ++i) {
            int32_t vector_size;
            if (recv(socket, &vector_size, sizeof(vector_size), 0) <= 0) {
                Log::recordError("Failed to receive vector size");
                return;
            }
            std::cout << "Received vector size: " << vector_size << std::endl;

            std::vector<int16_t> vec(vector_size);
            if (recv(socket, vec.data(), vector_size * sizeof(int16_t), 0) <= 0) {
                Log::recordError("Failed to receive vector values");
                return;
            }

            std::cout << "Received vector values: ";
            for (auto val : vec) {
                std::cout << val << " ";
            }
            std::cout << std::endl;

            int16_t result = processor.computeProduct(vec);  // Используем processor для вычисления
            send(socket, &result, sizeof(result), 0);
            std::cout << "Sent result: " << result << std::endl;
        }
    }
};

// Главная функция
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./server <log_file> <db_file>" << std::endl;
        return 1;
    }

    std::string logFile = argv[1];
    std::string dbFile = argv[2];
    int port = 33333;

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        Log::recordError("Socket creation error", true);
        return -1;
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        Log::recordError("Setsockopt error", true);
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        Log::recordError("Bind error", true);
        return -1;
    }

    if (listen(server_fd, 3) < 0) {
        Log::recordError("Listen error", true);
        return -1;
    }

    std::cout << "Server is running on port " << port << std::endl;
    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            Log::recordError("Accept error", true);
            return -1;
        }

        ClientHandler clientHandler;
        clientHandler.handleRequest(new_socket, dbFile, logFile);
        close(new_socket);
    }

    return 0;
}
