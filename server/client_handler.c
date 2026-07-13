#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "client_handler.h"


void *handle_client(void *socket)
{
    int client_socket = *(int *)socket;

    free(socket);


    char buffer[1024];


    while(1)
    {
        memset(buffer, 0, sizeof(buffer));


        int bytes_received = recv(
            client_socket,
            buffer,
            sizeof(buffer),
            0
        );


        if(bytes_received <= 0)
        {
            printf("Client disconnected\n");
            break;
        }



        printf(
            "Command: %s",
            buffer
        );



        if(strcmp(buffer, "monitor\n") == 0)
        {
            printf("Monitor command received\n");


            char response[] =
                "Monitor feature coming soon\n";


            send(
                client_socket,
                response,
                strlen(response),
                0
            );
        }


        else if(strcmp(buffer, "exit\n") == 0)
        {
            printf("Client requested exit\n");


            char response[] =
                "Goodbye\n";


            send(
                client_socket,
                response,
                strlen(response),
                0
            );


            break;
        }


        else
        {
            char response[] =
                "Unknown command\n";


            send(
                client_socket,
                response,
                strlen(response),
                0
            );
        }
    }


    close(client_socket);


    pthread_exit(NULL);
}