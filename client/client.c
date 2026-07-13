#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080


void remove_newline(char *str)
{
    str[strcspn(str, "\n")] = 0;
}


int main()
{
    int socket_fd;

    struct sockaddr_in server;


    char buffer[1024];
    char input[1024];



    socket_fd = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );


    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);


    inet_pton(
        AF_INET,
        "127.0.0.1",
        &server.sin_addr
    );


    connect(
        socket_fd,
        (struct sockaddr *)&server,
        sizeof(server)
    );



    // =====================
    // USERNAME
    // =====================

    memset(buffer,0,sizeof(buffer));


    recv(
        socket_fd,
        buffer,
        sizeof(buffer)-1,
        0
    );


    printf("%s: ", buffer);



    memset(input,0,sizeof(input));


    fgets(
        input,
        sizeof(input),
        stdin
    );


    send(
        socket_fd,
        input,
        strlen(input),
        0
    );



    // =====================
    // PASSWORD
    // =====================

    memset(buffer,0,sizeof(buffer));


    recv(
        socket_fd,
        buffer,
        sizeof(buffer)-1,
        0
    );


    printf("%s: ", buffer);



    memset(input,0,sizeof(input));


    fgets(
        input,
        sizeof(input),
        stdin
    );


    send(
        socket_fd,
        input,
        strlen(input),
        0
    );



    // =====================
    // LOGIN RESULT
    // =====================

    memset(buffer,0,sizeof(buffer));


    recv(
        socket_fd,
        buffer,
        sizeof(buffer)-1,
        0
    );


    printf(
        "%s\n",
        buffer
    );



    if(strncmp(buffer,"LOGIN_SUCCESS",13)!=0)
    {
        close(socket_fd);
        return 0;
    }



    // =====================
    // COMMAND LOOP
    // =====================

    while(1)
    {

        printf("> ");


        memset(input,0,sizeof(input));


        fgets(
            input,
            sizeof(input),
            stdin
        );


        send(
            socket_fd,
            input,
            strlen(input),
            0
        );


        memset(buffer,0,sizeof(buffer));


        recv(
            socket_fd,
            buffer,
            sizeof(buffer)-1,
            0
        );


        printf(
            "%s",
            buffer
        );


        if(strcmp(input,"exit\n")==0)
        {
            break;
        }

    }



    close(socket_fd);


    return 0;
}