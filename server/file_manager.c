#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "file_manager.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>

void list_files(
    char *buffer,
    int size
)
{

    DIR *dir;

    struct dirent *entry;


    dir = opendir(
        "server_files"
    );


    if(dir == NULL)
    {

        snprintf(
            buffer,
            size,
            "Cannot open server_files\n"
        );

        return;

    }



    strcpy(
        buffer,
        "Files:\n"
    );



    while(
        (entry = readdir(dir)) != NULL
    )
    {

        if(
            strcmp(entry->d_name,".") != 0 &&
            strcmp(entry->d_name,"..") != 0
        )
        {

            strcat(
                buffer,
                entry->d_name
            );


            strcat(
                buffer,
                "\n"
            );

        }

    }


    closedir(dir);

}

int receive_file(
    int client_socket,
    char *filename
)
{

    char path[512];


    snprintf(
        path,
        sizeof(path),
        "server_files/%s",
        filename
    );



    FILE *file = fopen(
        path,
        "wb"
    );


    if(file == NULL)
    {
        return 0;
    }



    long filesize;



    recv(
        client_socket,
        &filesize,
        sizeof(filesize),
        0
    );



    char buffer[4096];


    long received = 0;



    while(received < filesize)
    {

        int bytes = recv(
            client_socket,
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



    return 1;

}

int send_file(
    int client_socket,
    char *filename
)
{

    char path[512];


    snprintf(
        path,
        sizeof(path),
        "server_files/%s",
        filename
    );



    FILE *file = fopen(
        path,
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



    // ส่งขนาดไฟล์

    send(
        client_socket,
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
            client_socket,
            buffer,
            bytes,
            0
        );

    }



    fclose(file);


    return 1;

}