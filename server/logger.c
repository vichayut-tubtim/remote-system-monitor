#include <stdio.h>
#include <time.h>
#include <string.h>

#include "logger.h"


void write_log(
    char *message
)
{

    FILE *file = fopen(
        "logs/server.log",
        "a"
    );


    if(file == NULL)
    {
        return;
    }



    time_t now;

    time(&now);



    char *time_string =
        ctime(&now);



    time_string[
        strlen(time_string)-1
    ] = '\0';



    fprintf(
        file,
        "[%s] %s\n",
        time_string,
        message
    );



    fclose(file);

}