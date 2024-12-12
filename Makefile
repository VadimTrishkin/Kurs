CC = g++
CFLAGS = -std=c++17 -Wall -pedantic -g
LDFLAGS = -lssl -lcrypto

SRCS = main.cpp Log.cpp DatabaseConnector.cpp VectorProcessor.cpp ClientHandler.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) server_log.txt
