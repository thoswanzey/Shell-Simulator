#include "main.h"
#include "commands.h"

char *args[64];
char *paths[64];
int nargs;
char line[128];
char **env;

int main(int argc, char** argv, char **envp)
{
  env = envp;
  chdir(getenv("HOME"));
  print_path();
  while(1)
  {
    get_args();
    command_handler();
    memset(args, '\0', sizeof(args)); // clear args
  }
}

void print_path() {
  char* pathstr = getenv("PATH");

  printf("PATH: \n%s\n", (pathstr != 0) ? pathstr : "getenv returned NULL");

  // split path strings
  char *token = strtok(pathstr, ":");

  printf("PATH STRINGS:\n");

  int i = 0;
  
  // print each path string
  while(token != NULL) {
    paths[i++] = token;
    printf("%s ", token);
    token = strtok(NULL, ":");
  }

  printf("\n");
}

void get_args() { // retrieves args from input and saves to global array
  char cmd[128]; // 
  char cwd[512];
  int i = 0;

  // get current working directory
  getcwd(cwd, sizeof(cwd));

  
  printf(GRN "\ncs360: " RESET);
  if(cwd)
    printf(CYN "%s " RESET, cwd);
  else
    printf(CYN "*CWD ERROR* " RESET);
  printf("$ ");
  
  fgets(cmd, sizeof(cmd), stdin);
  line[strlen(line)-1] = 0;

  strcpy(line, ""); // initialize line with empty string
  strcat(line, cmd); // copy input from fgets into buffer

  // split up user input into args
  char *token = strtok(line, " ");

  while(token != NULL) {
    char *str = (char*)calloc(strlen(token), sizeof(char));
    strcpy(str, token);
    args[i] = str;
    token = strtok(NULL, " ");
    i++;
  }

  nargs = i;
  args[i-1][strlen(args[i-1])-1] = 0; // remove extra char from last arg

  args[i] = NULL; // terminating null pointer for execv
}