#ifndef _COMMANDS_H_
#define _COMMANDS_H_

    /* Includes */
    #include <stdio.h>
    #include <string.h>

    /*Command functions and calls*/
    char *cmds[ ] = {"exit", 0};
    int(*fptr[ ])()={ command_exit};

    /* Declarations */
    void command_parse();
    void command_exit();

#endif