CC = gcc

TARGET = server

SRC = server.c client_handler.c auth.c permission.c system_monitor.c process_monitor.c file_manager.c


all:
	gcc $(SRC) -pthread -o $(TARGET)


clean:
	rm -f $(TARGET)