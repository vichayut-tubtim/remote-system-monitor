#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>


#define PORT 8080


int main()
{
    int server_socket;
    int client_socket;

    struct sockaddr_in server_address;

    char buffer[1024] = {0};


    // 1. Create socket
    server_socket = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );


    if(server_socket < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }


    // 2. Configure server address

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);



    // 3. Bind socket

    if(bind(
        server_socket,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    ) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }



    // 4. Listen

    listen(server_socket, 5);


    printf("Server listening on port %d...\n", PORT);



    // 5. Accept client

    client_socket = accept(
        server_socket,
        NULL,
        NULL
    );


    printf("Client connected!\n");



    // 6. Receive message

    recv(
        client_socket,
        buffer,
        sizeof(buffer),
        0
    );


    printf("Client says: %s\n", buffer);



    close(client_socket);
    close(server_socket);


    return 0;
}