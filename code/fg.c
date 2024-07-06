#include "headers.h"
#include "fg.h"

void fgfg(int backgroundprocessesid[], int numberofbackgroundprocess, int backgroundprocessno)
{
    if(backgroundprocessno>numberofbackgroundprocess)
        perror("Invalid process no");
    
    int status;
    setpgid(backgroundprocessesid[backgroundprocessno - 1], getpgid(0)); // setting group id which has control of terminal as of now

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    tcsetpgrp(0, backgroundprocessesid[backgroundprocessno - 1]); // giving foreground control to child

    if (kill(backgroundprocessesid[backgroundprocessno - 1], SIGCONT) < 0)
    {
        perror("Error : Can't Kill");
    }

    waitpid(backgroundprocessesid[backgroundprocessno - 1], &status, WUNTRACED);

    tcsetpgrp(0, getpgid(0)); // regaining control

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

// Function to bring a background process to the foreground and change its state to Running
void foreground(int pid) {
    int status;
    if (waitpid(pid, &status, WUNTRACED) > 0) {
        if (WIFSTOPPED(status)) {
            printf("# brings process [%d] to foreground and changes its state to Running\n", pid);
        } else {
            printf("Process %d is not stopped.\n", pid);
        }
    } else {
        printf("No such process found.\n");
    }
}

// <_____________________________-----------------------------------------_________________________________________>
#include "fg.h"

#include "headers.h"

int fg(int pid) {
    printf("came___fg");
  int status;
  setpgid(pid, getpgid(0));

  signal(SIGTTIN, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);

  tcsetpgrp(0, pid);

  if (kill(pid, SIGCONT) < 0) perror("Error");

  waitpid(pid, &status, WUNTRACED);

  tcsetpgrp(0, getpgid(0));

  signal(SIGTTIN, SIG_DFL);
  signal(SIGTTOU, SIG_DFL);

  if (WIFSTOPPED(status)) return 0;

  return WIFEXITED(status) ? pid : -pid;
}