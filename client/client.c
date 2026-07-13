#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>


#define PORT 8080

int download_file(
    int socket_fd,
    char *filename
)
{

    long filesize;


    recv(
        socket_fd,
        &filesize,
        sizeof(filesize),
        0
    );



    char path[512];


    snprintf(
        path,
        sizeof(path),
        "client_downloads/%s",
        filename
    );



    FILE *file = fopen(
        path,
        "wb"
    );


    if(file == NULL)
    {
        printf("Cannot create file\n");
        return 0;
    }



    char buffer[4096];


    long received = 0;



    while(received < filesize)
    {

        int bytes = recv(
            socket_fd,
            buffer,
            sizeof(buffer),
            0
        );


        if(bytes <= 0)
        {
            break;
        }



        fwrite(
            buffer,
            1,
            bytes,
            file
        );



        received += bytes;

    }



    fclose(file);



    printf(
        "Downloaded: %s (%ld bytes)\n",
        filename,
        filesize
    );


    return 1;

}

int upload_file(
    int socket_fd,
    char *filename
)
{

    FILE *file = fopen(
        filename,
        "rb"
    );


    if(file == NULL)
    {
        return 0;
    }



    fseek(
        file,
        0,
        SEEK_END
    );


    long filesize = ftell(file);


    rewind(file);



    send(
        socket_fd,
        &filesize,
        sizeof(filesize),
        0
    );



    char buffer[4096];


    int bytes;



    while(
        (bytes=fread(
            buffer,
            1,
            sizeof(buffer),
            file
        )) > 0
    )
    {

        send(
            socket_fd,
            buffer,
            bytes,
            0
        );

    }



    fclose(file);


    return 1;

}




int main()
{

    int socket_fd;


    struct sockaddr_in server;


    char buffer[4096];

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
    // LOGIN
    // =====================


    memset(buffer,0,sizeof(buffer));


    recv(
        socket_fd,
        buffer,
        sizeof(buffer)-1,
        0
    );


    printf(
        "%s: ",
        buffer
    );


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
        "%s: ",
        buffer
    );



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



        memset(
            input,
            0,
            sizeof(input)
        );


        fgets(
            input,
            sizeof(input),
            stdin
        );


        // =====================
        // DOWNLOAD
        // =====================


        if(strncmp(input,"download ",9)==0)
        {

            char filename[256];


            strcpy(
                filename,
                input + 9
            );


            filename[strcspn(filename,"\n")] = 0;



            send(
                socket_fd,
                input,
                strlen(input),
                0
            );



            memset(
                buffer,
                0,
                sizeof(buffer)
            );



            recv(
                socket_fd,
                buffer,
                sizeof(buffer)-1,
                0
            );

            buffer[strcspn(buffer,"\0")] = 0;



            if(strcmp(buffer,"DOWNLOAD_READY")==0)
            {

                download_file(
                    socket_fd,
                    filename
                );

            }
            else
            {

                printf(
                    "%s\n",
                    buffer
                );

            }


            continue;

        }



        // =====================
        // UPLOAD
        // =====================


        if(strncmp(input,"upload ",7)==0)
        {

            char filename[256];


            strcpy(
                filename,
                input+7
            );


            filename[strcspn(filename,"\n")]=0;



            FILE *check=fopen(
                filename,
                "rb"
            );


            if(check==NULL)
            {

                printf(
                    "File not found\n"
                );

                continue;

            }


            fclose(check);



            // ส่ง command

            send(
                socket_fd,
                input,
                strlen(input),
                0
            );



            memset(
                buffer,
                0,
                sizeof(buffer)
            );



            recv(
                socket_fd,
                buffer,
                sizeof(buffer)-1,
                0
            );



            if(strcmp(buffer,"READY_UPLOAD")==0)
            {

                upload_file(
                    socket_fd,
                    filename
                );



                memset(
                    buffer,
                    0,
                    sizeof(buffer)
                );



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

            }


            continue;

        }




        // =====================
        // NORMAL COMMAND
        // =====================


        send(
            socket_fd,
            input,
            strlen(input),
            0
        );



        memset(
            buffer,
            0,
            sizeof(buffer)
        );



        int bytes = recv(
            socket_fd,
            buffer,
            sizeof(buffer)-1,
            0
        );



        if(bytes <= 0)
        {
            break;
        }



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