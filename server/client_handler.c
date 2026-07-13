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



    char buffer[1024];

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
        8,
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


    buffer[bytes] = '\0';



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
        8,
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



    buffer[bytes] = '\0';



    strcpy(
        password,
        buffer
    );


    remove_newline(password);




    // =====================
    // AUTHENTICATION
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



        buffer[bytes] = '\0';



        printf(
            "[Client #%d] %s",
            client_id,
            buffer
        );



        // copy command สำหรับ check permission

        char command[100];


        strcpy(
            command,
            buffer
        );


        remove_newline(command);



        // =====================
        // PERMISSION CHECK
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

        else if(strcmp(command,"download")==0)
        {

            char response[] =
                "Download feature coming soon\n";


            send(
                client_socket,
                response,
                strlen(response),
                0
            );

        }



        else if(strcmp(command,"upload")==0)
        {

            char response[] =
                "Upload feature coming soon\n";


            send(
                client_socket,
                response,
                strlen(response),
                0
            );

        }



        else if(strcmp(command,"delete")==0)
        {

            char response[] =
                "Delete feature coming soon\n";


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