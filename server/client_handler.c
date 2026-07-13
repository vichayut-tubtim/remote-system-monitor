#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "client_handler.h"

#include <string.h>
#include <sys/socket.h>

void *handle_client(void *socket)
{
    int client_socket = *(int *)socket;

    free(socket);

    char buffer[1024] = {0};


    recv(
        client_socket,
        buffer,
        sizeof(buffer),
        0
    );


    if(strcmp(buffer,"monitor\n")==0)
    {
        printf("Monitor command received\n");
    }
    char response[] = "Monitor feature coming soon\n";

    send(
        client_socket,
        response,
        strlen(response),
        0
    );

    else if(strcmp(buffer,"exit\n")==0)
    {
        printf("Client requested exit\n");
    }
    else
    {
        printf("Unknown command: %s", buffer);
    }

    close(client_socket);


    pthread_exit(NULL);
}