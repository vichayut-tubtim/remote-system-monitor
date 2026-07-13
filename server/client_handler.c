#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "client_handler.h"



void *handle_client(void *data)
{
    ClientInfo *client = (ClientInfo *)data;


    int client_socket = client->socket;
    int client_id = client->id;


    free(client);



    char buffer[1024];



    printf(
        "[Client #%d] Session started\n",
        client_id
    );



    while(1)
    {

        memset(
            buffer,
            0,
            sizeof(buffer)
        );


        int bytes_received = recv(
            client_socket,
            buffer,
            sizeof(buffer),
            0
        );



        if(bytes_received <= 0)
        {
            printf(
                "[Client #%d] Disconnected\n",
                client_id
            );

            break;
        }



        printf(
            "[Client #%d] Command: %s",
            client_id,
            buffer
        );



        if(strcmp(buffer,"monitor\n")==0)
        {

            char response[] =
                "Monitor feature coming soon\n";


            send(
                client_socket,
                response,
                strlen(response),
                0
            );

        }


        else if(strcmp(buffer,"exit\n")==0)
        {

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