#include "Prompt.h"
#include "headers.h"

char *username = NULL;
char *systemname = NULL;
char initdir[256] = "";
char prevdir[256] = "";
int shellpid = 0;

void initialise() {
    struct utsname uts;
    uname(&uts);

    username = getenv("LOGNAME");
    systemname = strdup(uts.nodename); // Copy the system name directly

    getcwd(initdir, sizeof(initdir));
    strcpy(prevdir, initdir);
    shellpid = getpid();
}

void prompt() {
    char buff[256];
    getcwd(buff, sizeof(buff));

    char curdir[100] = "~";

    if (strcmp(buff, initdir) != 0) {
        strcpy(curdir, buff);
    } else {
        strcat(curdir, &buff[strlen(initdir)]);
    }

    printf("\033[1;32m<%s@\033[0m", username);
    printf("\033[1;33m%s:\033[0m", systemname);
    printf("\033[1;36m%s\033[0m", curdir);
}
