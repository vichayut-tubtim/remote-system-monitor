#include <stdio.h>
#include <string.h>

#include "auth.h"


int authenticate(
    char *username,
    char *password,
    char *role
)
{

    FILE *file = fopen(
        "users.txt",
        "r"
    );


    if(file == NULL)
    {
        return 0;
    }



    char u[50];
    char p[50];
    char r[20];



    while(
        fscanf(
            file,
            "%s %s %s",
            u,
            p,
            r
        ) == 3
    )
    {

        if(
            strcmp(username,u)==0 &&
            strcmp(password,p)==0
        )
        {

            strcpy(
                role,
                r
            );


            fclose(file);

            return 1;

        }

    }


    fclose(file);


    return 0;

}