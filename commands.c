#include "commands.h"

extern char *args[64];

char *cmds[ ] = {"exit", 0};
int(*fptr[ ])()={ command_exit};

void command_exit() 
{
    printf("Program exit.\n");
    exit(0);
}

void command_parse() 
{   
    int i;
    for(i = 0; cmds[i]; i++)
    {
        if(!strcmp(args[0], cmds[i]))
        {
            fptr[i]();
            i = -1;
            break;
        }
    }

    if(i != -1)
        printf("invalid command\n");
}