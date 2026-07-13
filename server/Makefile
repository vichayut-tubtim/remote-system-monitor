CC = gcc

TARGET = server

SRC = server.c client_handler.c auth.c


all:
	gcc $(SRC) -pthread -o $(TARGET)


clean:
	rm -f $(TARGET)