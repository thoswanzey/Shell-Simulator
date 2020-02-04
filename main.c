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
  print_path(); // print path
  while(1)
  {
    get_args(); // get args
    command_parse();
    memset(args, '\0', sizeof(args)); // clear args
  }
}

void print_path() {
  char* s = getenv("PATH");
  printf("PATH: \n%s\n", (s != 0) ? s : "getenv returned NULL");

  char *token = strtok(s, ":");

  printf("PATH STRINGS:\n");
  int i = 0;
  while(token != NULL) {
    paths[i++] = token;
    printf("%s ", token);
    token = strtok(NULL, ":");
  }

  printf("\n");
}

void get_args() { // retrieves args from input and saves to global array
    char temp[128];
    int i = 0;

    printf("\ncs360 $ ");
		fgets(temp, sizeof(temp), stdin);
		line[strlen(line)-1] = 0;
		
    strcpy(line, "");
    strcat(line, temp);

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