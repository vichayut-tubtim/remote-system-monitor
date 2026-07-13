#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include <string.h>
#include <arpa/inet.h>

#include <sys/socket.h>
#include <netinet/in.h>


#include "client_handler.h"
#include "logger.h"

#define PORT 8080



int main()
{

    int server_socket;
    int client_socket;


    struct sockaddr_in server_address;
    struct sockaddr_in client_address;


    socklen_t client_size;



    int client_id = 1;



    server_socket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );



    if(server_socket < 0)
    {
        perror("Socket failed");
        exit(1);
    }



    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);



    bind(
        server_socket,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    );



    listen(server_socket,5);



    printf(
        "Server listening on port %d...\n",
        PORT
    );



    while(1)
    {

        client_size = sizeof(client_address);



        client_socket = accept(
            server_socket,
            (struct sockaddr *)&client_address,
            &client_size
        );



        printf(
            "New client connected: #%d\n",
            client_id
        );

        char log[100];

        sprintf(
            log,
            "Client #%d connected",
            client_id
        );

        write_log(log);


        ClientInfo *client =
            malloc(sizeof(ClientInfo));



        client->socket = client_socket;
        client->id = client_id;



        pthread_t thread;



        pthread_create(
            &thread,
            NULL,
            handle_client,
            client
        );



        pthread_detach(thread);



        client_id++;

    }



    close(server_socket);


    return 0;
}