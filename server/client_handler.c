#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>

#include "client_handler.h"
#include "auth.h"
#include "permission.h"
#include "system_monitor.h"
#include "process_monitor.h"
#include "file_manager.h"
#include "logger.h"

void remove_newline(char *str)
{
    str[strcspn(str, "\n")] = 0;
}



void *handle_client(void *data)
{

    ClientInfo *client = (ClientInfo *)data;


    int client_socket = client->socket;
    int client_id = client->id;


    free(client);



    char buffer[4096];

    char username[50];
    char password[50];
    char role[20];



    printf(
        "[Client #%d] Connected\n",
        client_id
    );



    // =====================
    // LOGIN USERNAME
    // =====================


    send(
        client_socket,
        "USERNAME",
        strlen("USERNAME"),
        0
    );


    memset(buffer,0,sizeof(buffer));


    int bytes = recv(
        client_socket,
        buffer,
        sizeof(buffer)-1,
        0
    );


    if(bytes <= 0)
    {
        close(client_socket);
        return NULL;
    }


    buffer[bytes]='\0';


    strcpy(
        username,
        buffer
    );


    remove_newline(username);



    // =====================
    // LOGIN PASSWORD
    // =====================


    send(
        client_socket,
        "PASSWORD",
        strlen("PASSWORD"),
        0
    );



    memset(buffer,0,sizeof(buffer));


    bytes = recv(
        client_socket,
        buffer,
        sizeof(buffer)-1,
        0
    );


    if(bytes <= 0)
    {
        close(client_socket);
        return NULL;
    }


    buffer[bytes]='\0';



    strcpy(
        password,
        buffer
    );


    remove_newline(password);



    // =====================
    // AUTH
    // =====================


    if(authenticate(
        username,
        password,
        role
    ))
    {

        char response[100];


        snprintf(
            response,
            sizeof(response),
            "LOGIN_SUCCESS %s",
            role
        );


        send(
            client_socket,
            response,
            strlen(response),
            0
        );


        printf(
            "[Client #%d] Login %s (%s)\n",
            client_id,
            username,
            role
        );

        char log[200];

        sprintf(
            log,
            "Client #%d user %s login success (%s)",
            client_id,
            username,
            role
        );

        write_log(log);

    }

    else
    {

        char response[] =
            "LOGIN_FAILED";


        send(
            client_socket,
            response,
            strlen(response),
            0
        );


        close(client_socket);

        return NULL;

    }



    // =====================
    // COMMAND LOOP
    // =====================


    while(1)
    {

        memset(
            buffer,
            0,
            sizeof(buffer)
        );


        bytes = recv(
            client_socket,
            buffer,
            sizeof(buffer)-1,
            0
        );


        if(bytes <= 0)
        {
            printf(
                "[Client #%d] Disconnected\n",
                client_id
            );

            break;
        }



        buffer[bytes]='\0';



        remove_newline(buffer);



        printf(
            "[Client #%d] Command: %s\n",
            client_id,
            buffer
        );



        // แยก command กับ argument

        char command[100];
        char argument[256];


        memset(command,0,sizeof(command));
        memset(argument,0,sizeof(argument));


        sscanf(
            buffer,
            "%s %s",
            command,
            argument
        );



        // =====================
        // PERMISSION
        // =====================


        if(!check_permission(
            role,
            command
        ))
        {

            char response[] =
                "Permission denied\n";


            send(
                client_socket,
                response,
                strlen(response),
                0
            );


            continue;

        }



        // =====================
        // COMMAND EXECUTION
        // =====================



        if(strcmp(command,"monitor")==0)
        {

            char response[1024];


            memset(
                response,
                0,
                sizeof(response)
            );


            get_system_status(
                response,
                sizeof(response)
            );


            send(
                client_socket,
                response,
                strlen(response),
                0
            );

        }



        else if(strcmp(command,"processes")==0)
        {

            char response[4096];


            memset(
                response,
                0,
                sizeof(response)
            );


            get_process_list(
                response,
                sizeof(response)
            );


            send(
                client_socket,
                response,
                strlen(response),
                0
            );

        }



        else if(strcmp(command,"list")==0)
        {

            char response[4096];


            memset(
                response,
                0,
                sizeof(response)
            );


            list_files(
                response,
                sizeof(response)
            );


            send(
                client_socket,
                response,
                strlen(response),
                0
            );

        }



        else if(strcmp(command,"upload")==0)
        {

            if(strlen(argument)==0)
            {

                char response[] =
                    "Usage: upload filename\n";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );


                continue;

            }



            printf(
                "[Client #%d] Upload request: %s\n",
                client_id,
                argument
            );



            send(
                client_socket,
                "READY_UPLOAD",
                strlen("READY_UPLOAD"),
                0
            );



            if(receive_file(
                client_socket,
                argument
            ))
            {

                char response[] =
                    "Upload success\n";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );

            }
            else
            {

                char response[] =
                    "Upload failed\n";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );

            }

        }



        else if(strcmp(command,"download")==0)
        {

            if(strlen(argument)==0)
            {

                char response[] =
                    "Usage: download filename\n";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );


                continue;

            }



            char path[512];


            snprintf(
                path,
                sizeof(path),
                "server_files/%s",
                argument
            );



            FILE *check = fopen(
                path,
                "rb"
            );


            if(check == NULL)
            {

                char response[] =
                    "FILE_NOT_FOUND";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );


                continue;

            }


            fclose(check);



            // บอก client ว่าพร้อม

            send(
                client_socket,
                "DOWNLOAD_READY",
                strlen("DOWNLOAD_READY"),
                0
            );



            // ส่งไฟล์

            send_file(
                client_socket,
                argument
            );

        }



        else if(strcmp(command,"delete")==0)
        {

            if(strlen(argument)==0)
            {

                char response[] =
                    "Usage: delete filename\n";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );


                continue;

            }



            if(delete_file(argument))
            {

                char response[] =
                    "Delete success\n";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );

            }
            else
            {

                char response[] =
                    "Delete failed\n";


                send(
                    client_socket,
                    response,
                    strlen(response),
                    0
                );

            }

        }



        else if(strcmp(command,"exit")==0)
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


    return NULL;

}