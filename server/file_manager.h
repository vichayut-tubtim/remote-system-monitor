#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H


void list_files(
    char *buffer,
    int size
);


int receive_file(
    int client_socket,
    char *filename
);


int send_file(
    int client_socket,
    char *filename
);


#endif