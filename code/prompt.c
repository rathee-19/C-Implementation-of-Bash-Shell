#include "prompt.h"
#include "headers.h"

// char *username = NULL;
// char *systemname = NULL;
char initdir[256] = "";
char prevdir[256] = "";
int shellpid = 0;

// void initialise() {
//     username = getenv("USER");
    
//     systemname = (char *)calloc(1024, sizeof(char));
//     gethostname(systemname, 1024);
//     printf("%s", systemname);
//     getcwd(initdir, sizeof(initdir));
//     strcpy(prevdir, initdir);
//     shellpid = getpid();
// }

void prompt(char * username, char * systemname) {
    // printf("after__>signal");
    char buff[256];
    getcwd(buff, sizeof(buff));

    char curdir[100] = "~";

    strcat(curdir, &buff[strlen(initdir)]);
    // printf("\n");
    printf("\033[1;32m<%s@\033[0m", username);
    printf("\033[1;33m%s:\033[0m", systemname);
    printf("\033[1;36m%s \033[0m", curdir);
}
