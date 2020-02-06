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


void command_handler()
{
    //Check if it is cd or exit command before forking new process
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

    //If not cd or exit, pass it to the pipe_handler
    if(i != -1)
    {
        int pid;
        pid = fork();
        if(pid)
        {
            //parent executes this part
            int dead_pid, exitval;
            //wait for child process to terminate
            dead_pid = wait(&exitval);
            printf(YEL "\nProcess %d exited with code %d\n" RESET, dead_pid, exitval);
        }
        else
        {
            //child executes this part
            if(!pipe_handler() && !redirect_handler())
                command_parse(args);
        }
    }
}


int redirect_handler() // return 1 if redirect is found
{
    int fd;
    char fileRedirect[512]; // filewrite name
    char *exec_args[64]; // used to filter out redirection
    
    
    /* create a new set of args called exec_args that exclude io redirection */
    int i = 0;
    int hasRedirect = 0;
    while(args[i]) 
    {
        if(!strcmp(args[i], ">") || !strcmp(args[i], ">>") || !strcmp(args[i], "<"))
        {
            hasRedirect = -1;
            break;
        }
        
        exec_args[i] = args[i]; // copies args up to > , >>, or <

        i++;
    }
    
    exec_args[i] = NULL; // terminating null pointer for execv


    if(hasRedirect < 0) // redirect to file
    {
        
        getcwd(fileRedirect, sizeof(fileRedirect));
        strcat(fileRedirect, "/");
        strcat(fileRedirect, args[i+1]);

        if(!strcmp(args[i], ">")) {
            fd = open(fileRedirect, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
            dup2(fd, 1); // copy stdout fd to file
        } else if(!strcmp(args[i], ">>")) {
            fd = open(fileRedirect, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
            dup2(fd, 1); // copy stdout fd to file
        } else {
            fd = open(fileRedirect, O_RDONLY);
            dup2(fd, 0); // copy file to stdin
        }

        if (fd < 0) 
            printf("Could not create file: %s\n", args[i+1]);


        close(fd);
        
        command_parse(exec_args);
        return 1;
    }
    
    return 0;
}

int pipe_handler() // return 1 if pipe is found
{
    int i = 0, isPipe = 0;
    char * head_args[64];
    char * tail_args[64];

    //check if command has pipe
    while(args[i]) 
    {
        if(!strcmp(args[i], "|"))
        {
            isPipe = 1;
            break;
        }
        
        head_args[i] = args[i]; // copies args up to > or >>
        i++;
    }
    head_args[i] = NULL;

    //If command includes pipe, find head and tail arguments to pass to command_parse
    if(isPipe)
    {
        i++;
        int j = 0;
        while(args[i + j])
        {
            tail_args[j] = args[i + j];
            j++;
        }
        tail_args[j] = NULL;

        int pid, pd[2];
        pipe(pd);
        pid = fork();
        if(pid)
        {   
            //(Fork parent) Reading process
            close(pd[1]);
            close(0);
            dup(pd[0]);
            close(pd[0]);
            command_parse(tail_args);
        }
        else
        {
            //(Fork Child) Writing Process
            close(pd[0]);
            close(1);
            dup(pd[1]);
            close(pd[1]);
            command_parse(head_args);

        }

        return 1;
    }
    
    return 0;
    
}


void command_parse(char * _args[]) 
{   
        
    char filepath[512];
    int j = 0;
    while(paths[j])
    {
        strcpy(filepath, paths[j++]);
        strcat(filepath, "/");
        strcat(filepath, _args[0]);
        if(access(filepath, F_OK) != -1)
        {
            j = -1;
            break;
        }
    }

    //If executible file was found
    if(j == -1)
    {
            
        execve(filepath, _args, env);
        
        //execve should never return since it replaces the calling process
        //if it returns, tell terminal an error occured and exit process
        printf("**ERROR**\n");
        exit(-1);
    }

    //Command not found in command array or as executible in all environmental variable paths so exit new process
    else
    {
        printf("Invalid Command\n");
        exit(0);
    }
}