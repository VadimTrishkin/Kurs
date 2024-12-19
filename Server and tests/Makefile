# Компилятор и флаги
CC = g++
CFLAGS = -std=c++17 -Wall -pedantic -g
LDFLAGS = -lssl -lcrypto -lUnitTest++

# Директории
DB_FILE = database.txt         # Путь к файлу базы данных (обновлено)
SRCS = main.cpp Log.cpp DatabaseConnector.cpp VectorProcessor.cpp ClientHandler.cpp  # Все исходные файлы
OBJS = $(SRCS:.cpp=.o)       # Преобразуем .cpp файлы в .o файлы
EXEC = server                 # Имя исполнимого файла для сервера

# Исходники для тестов
TEST_SRCS = test_all.cpp Log.cpp DatabaseConnector.cpp VectorProcessor.cpp ClientHandler.cpp  # Все исходные файлы
TEST_OBJS = $(TEST_SRCS:.cpp=.o)  # Преобразуем .cpp файлы для тестов в .o файлы
TEST_EXEC = test_all            # Имя исполнимого файла для тестов

# Правила сборки
all: $(EXEC) $(TEST_EXEC)

# Линковка объектов в исполнимый файл для сервера
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

# Линковка объектов в исполнимый файл для тестов
$(TEST_EXEC): $(TEST_OBJS)
	$(CC) $(TEST_OBJS) -o $(TEST_EXEC) $(LDFLAGS)

# Правило для компиляции .cpp файлов в .o файлы для сервера
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для компиляции .cpp файлов в .o файлы для тестов
test_all.o: test_all.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка объектов и исполнимых файлов
clean:
	rm -f $(OBJS) $(EXEC) $(TEST_OBJS) $(TEST_EXEC)

# Запуск сервера с базой данных
run: $(EXEC)
	./$(EXEC) $(DB_FILE)

# Запуск тестов
run_tests: $(TEST_EXEC)
	./$(TEST_EXEC)
