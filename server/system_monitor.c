#include <stdio.h>
#include <string.h>

#include "system_monitor.h"



void get_system_status(
    char *buffer,
    int size
)
{

    FILE *file;


    char line[256];


    long total_memory = 0;
    long free_memory = 0;



    file = fopen(
        "/proc/meminfo",
        "r"
    );



    if(file != NULL)
    {

        while(
            fgets(
                line,
                sizeof(line),
                file
            )
        )
        {

            if(
                sscanf(
                    line,
                    "MemTotal: %ld",
                    &total_memory
                )
            )
            {
                continue;
            }


            if(
                sscanf(
                    line,
                    "MemAvailable: %ld",
                    &free_memory
                )
            )
            {
                break;
            }

        }


        fclose(file);

    }



    snprintf(
        buffer,
        size,

        "System Monitor\n"
        "================\n"
        "Total Memory: %ld MB\n"
        "Available Memory: %ld MB\n",

        total_memory / 1024,
        free_memory / 1024
    );

}