#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#include "process_monitor.h"



void get_process_list(
    char *buffer,
    int size
)
{

    DIR *dir;

    struct dirent *entry;


    dir = opendir("/proc");


    if(dir == NULL)
    {
        snprintf(
            buffer,
            size,
            "Cannot open /proc\n"
        );

        return;
    }



    strcpy(
        buffer,
        "PID      COMMAND                   STATE\n"
    );


    strcat(
        buffer,
        "--------------------------------------------\n"
    );


    while(
        (entry = readdir(dir)) != NULL
    )
    {

        int numeric = 1;



        for(
            int i = 0;
            entry->d_name[i];
            i++
        )
        {

            if(!isdigit(entry->d_name[i]))
            {
                numeric = 0;
                break;
            }

        }



        if(numeric)
        {

            char path[512];


            snprintf(
                path,
                sizeof(path),
                "/proc/%.240s/status",
                entry->d_name
            );



            FILE *file = fopen(
                path,
                "r"
            );



            if(file)
            {

                char line[256];


                char name[100] = "";
                char state[50] = "";
                char command[200] = "";



                while(
                    fgets(
                        line,
                        sizeof(line),
                        file
                    )
                )
                {

                    sscanf(
                        line,
                        "Name:\t%99s",
                        name
                    );


                    sscanf(
                        line,
                        "State:\t%49s",
                        state
                    );

                }


                fclose(file);



                // Read real command

                char cmd_path[512];


                snprintf(
                    cmd_path,
                    sizeof(cmd_path),
                    "/proc/%.240s/cmdline",
                    entry->d_name
                );



                FILE *cmd_file = fopen(
                    cmd_path,
                    "r"
                );



                if(cmd_file)
                {

                    fread(
                        command,
                        1,
                        sizeof(command)-1,
                        cmd_file
                    );


                    fclose(cmd_file);

                }



                // Some kernel processes have no cmdline

                if(strlen(command)==0)
                {
                    strcpy(
                        command,
                        name
                    );
                }



                char process[512];



                snprintf(
                    process,
                    sizeof(process),
                    "%-8s %-25s %-10s\n",
                    entry->d_name,
                    command,
                    state
                );


                if(strlen(buffer) + strlen(process) < size)
                {
                    strcat(
                        buffer,
                        process
                    );
                }

            }

        }

    }



    closedir(dir);

}