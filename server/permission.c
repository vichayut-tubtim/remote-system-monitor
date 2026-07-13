#include <string.h>

#include "permission.h"


int check_permission(
    char *role,
    char *command
)
{

    // ทุกคน logout ได้

    if(strcmp(command,"exit")==0)
    {
        return 1;
    }



    // admin ทำได้ทุกอย่าง

    if(strcmp(role,"admin")==0)
    {
        return 1;
    }



    // user

    if(strcmp(role,"user")==0)
    {

        if(
            strcmp(command,"monitor")==0 ||
            strcmp(command,"download")==0 ||
            strcmp(command,"processes")==0 ||
            strcmp(command,"upload")==0 ||
            strcmp(command,"list")==0
        )
        {
            return 1;
        }

    }



    return 0;

}