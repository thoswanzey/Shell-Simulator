#include "commands.h"

extern *args[64];

void command_exit() 
{
    printf("Program exit.\n");
    exit(0);
}

void command_parse() 
{   
    if(strcmp(args[0], "exit") == 0)
        command_exit();
}