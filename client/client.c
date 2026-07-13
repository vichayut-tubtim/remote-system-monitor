#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>


#define PORT 8080


int main()
{
    int socket_fd;

    struct sockaddr_in server_address;


    char message[1024];
    char buffer[1024];



    socket_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );


    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);


    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server_address.sin_addr
    );



    connect(
        socket_fd,
        (struct sockaddr *)&server_address,
        sizeof(server_address)
    );


    while(1)
    {
        memset(message,0,sizeof(message));


        printf("> ");


        fgets(
            message,
            sizeof(message),
            stdin
        );


        send(
            socket_fd,
            message,
            strlen(message),
            0
        );



        memset(buffer,0,sizeof(buffer));


        recv(
            socket_fd,
            buffer,
            sizeof(buffer),
            0
        );


        printf(
            "Server: %s",
            buffer
        );



        if(strcmp(message,"exit\n")==0)
        {
            break;
        }
    }


    close(socket_fd);


    return 0;
}