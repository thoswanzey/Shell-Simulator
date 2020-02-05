#ifndef _COMMANDS_H_
#define _COMMANDS_H_

    /* Includes */
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <unistd.h>
    #include <wait.h>

    /* FANCY */
    #define RED   "\x1B[31m"
    #define GRN   "\x1B[32m"
    #define YEL   "\x1B[33m"
    #define BLU   "\x1B[34m"
    #define MAG   "\x1B[35m"
    #define CYN   "\x1B[36m"
    #define WHT   "\x1B[37m"
    #define RESET "\x1B[0m"


    /* Declarations */
    void command_cd();
    void command_exit();
    void command_parse();
    
#endif