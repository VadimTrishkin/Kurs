
CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# Путь к библиотеке UnitTest++ и OpenSSL
UNITTEST_INCLUDE = /usr/include/UnitTest++/
UNITTEST_LIB = -lUnitTest++ -lssl -lcrypto

# Исходный файл с тестами
TEST_FILE = test_all.cpp

# Название исполнимого файла
TARGET = test_program

# Путь к заголовочным файлам
INCLUDES = -I$(UNITTEST_INCLUDE)


all: $(TARGET)


$(TARGET): $(TEST_FILE)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(UNITTEST_LIB)


test: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET)


distclean: clean
	rm -f test_log.txt test_db.txt

