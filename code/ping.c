#include "headers.h"


int kill_job(int pid, int sig)
{
    // printf("signal_check-->%d",sig);
    if (kill(pid, sig) < 0)
    {
        perror("Error");
        return -1;
    }

    return 0;
}
