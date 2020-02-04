#include "commands.h"

extern char *args[64];
extern char line[128];
extern char *paths[64];
extern char **env;

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
    //If command wasn't found in the defined command array
    if(i != -1)
    {
        char filepath[512];
        int j = 0;
        while(paths[j])
        {
            strcpy(filepath, paths[j++]);
            strcat(filepath, "/");
            strcat(filepath, args[0]);
            //printf("FILEPATH: %s\n", filepath);                       //**DEBUG** : prints all tested filepaths
            if(access(filepath, F_OK) != -1)
            {
                //printf("FILEPATH LOCATED:: %s\n\n", filepath);        //**DEBUG** : prints valid filepath if found
                j = -1;
                break;
            }
        }

        //If executible file was foundall tested filepaths
        if(j == -1)
        {
            int pid;
            pid = fork();
            if(pid)
            {
                //parent executes this part
                int dead_pid, exitval;
                //wait for child process to terminate
                dead_pid = wait(&exitval);
                printf("Process %d exited with code %d\n", dead_pid, exitval);
            }
            else
            {
                //child Executes this part;
                execve(filepath, args, env);
                //execve should never return since it replaces the calling process
                //if it returns, tell terminal an error occured and exit process
                printf("**ERROR**");
                exit(-1);
            }
        }

        //Command not found in command array or as executible in all environmental variable paths
        else
        {
            printf("Invalid Command\n");
        }
        

    }
    //TODO  recognize invalid command and print to terminal
}