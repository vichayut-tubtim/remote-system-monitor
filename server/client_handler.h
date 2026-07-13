#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H


typedef struct
{
    int socket;
    int id;

} ClientInfo;


void *handle_client(void *client);


#endif