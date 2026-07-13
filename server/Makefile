CC = gcc

TARGET = server

SRC = server.c client_handler.c


all:
	gcc $(SRC) -pthread -I../common -o $(TARGET)


clean:
	rm -f $(TARGET)