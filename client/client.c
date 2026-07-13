#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>


#define PORT 8080


int main()
{
    int socket_fd;

    struct sockaddr_in server_address;


    char message[] = "Hello Server";



    // 1. Create socket

    socket_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );



    // 2. Server address

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);



    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server_address.sin_addr
    );



    // 3. Connect

    connect(
        socket_fd,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    );



    // 4. Send message

    send(
        socket_fd,
        message,
        strlen(message),
        0
    );


    printf("Message sent\n");


    close(socket_fd);


    return 0;
}