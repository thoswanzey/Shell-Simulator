#include "commands.h"

extern int nargs;
extern char *args[64];
extern char line[128];
extern char *paths[64];
extern char **env;

char *cmds[ ] = {"exit", "cd",  0};
int (*fptr[ ])() = { command_exit, command_cd };

void command_cd()
{
    int fail = -1;
    if(nargs == 2)
    {
        fail = chdir(args[1]);
    }
    else if(nargs == 1)
    {
        fail = chdir(getenv("HOME"));
    }

    //Will trigger if wrong number of parameters or failed chdir call
    if(fail)
    {
        printf("Invalid Command\n");
    }
}


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

        //If executible file was found
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
                
                int fd, j;
                char filewrite[512]; // filewrite name
                char *exec_args[64]; // used to filter out redirection
                
                
                /* create a new set of args called exec_args that exclude io redirection */
                i = 0;
                while(args[i]) 
                {
                    if(!strcmp(args[i], ">") || !strcmp(args[i], ">>") || !strcmp(args[i], "<"))
                    {
                        j = -1;
                        break;
                    }
                    
                    exec_args[i] = args[i]; // copies args up to > or >>

                    i++;
                }
                
                exec_args[i] = NULL; // terminating null pointer for execv


                if(j < 0) // redirect to file
                {
                    
                    getcwd(filewrite, sizeof(filewrite));
                    strcat(filewrite, "/");
                    strcat(filewrite, args[i+1]);

                    if(!strcmp(args[i], ">")) {
                        fd = open(filewrite, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                        dup2(fd, 1); // copy stdout fd to file
                    } else if(!strcmp(args[i], ">>")) {
                        fd = open(filewrite, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
                        dup2(fd, 1); // copy stdout fd to file
                    } else {
                        fd = open(filewrite, O_RDONLY);
                        dup2(fd, 0); // copy file to stdin
                    }

                    if (fd < 0) 
                        printf("Could not create file: %s\n", args[i+1]);


                    close(fd);
                    
                    execve(filepath, exec_args, env);
                    
                }
                else
                {
                    execve(filepath, args, env);
                }
                
                //execve should never return since it replaces the calling process
                //if it returns, tell terminal an error occured and exit process
                printf("**ERROR**\n");
                exit(-1);
            }
        }

        //Command not found in command array or as executible in all environmental variable paths
        else
        {
            printf("Invalid Command\n");
        }
    }
}