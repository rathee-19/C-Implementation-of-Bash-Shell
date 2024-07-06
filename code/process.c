#include "headers.h"
#include "process.h"


void execute_process(int num_args, char **args, int background_flag, int *proc_no, pid_t *background_processes, char **backgroundCommand, int *backgroundprocessesid) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        // Child process
        if (background_flag) {
            // If it's a background process, redirect stdin from /dev/null
            int devnull_fd = open("/dev/null", O_RDONLY);
            dup2(devnull_fd, 0);
            close(devnull_fd);
        }

        // Execute the command
        execvp(args[0], args);

        // If execvp fails, print an error message and exit
        perror("execvp");
        exit(1);
    } else {
        // Parent process
        if (background_flag) {
            // Background process
            if ((*proc_no) < MAX_BACKGROUND_PROCESSES) {
                background_processes[(*proc_no)] = pid;
                (*proc_no)++;
                int pos = *proc_no;
                backgroundprocessesid[pos] = pid;
                backgroundCommand[pos] = strdup(args[0]);
                printf("[%d] %d\n", pos, pid); // Print background job information
            } else {
                printf("Too many background processes, cannot add another.\n");
            }
        } else {
            // Foreground process
            printf("coming or foreground in process.c");
            int status;
            foregroundProcessPid = pid;
            printf("\n%d",foregroundProcessPid);
            // Set up signal handlers for job control
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);
            tcsetpgrp(0, pid);

            // Wait for the foreground process to complete or be stopped
            waitpid(pid, &status, WUNTRACED);

            tcsetpgrp(0, getpgid(0));
            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

            foregroundProcessPid = 0;

            if (WIFSTOPPED(status)) {
                // Process was stopped
                printf("Process stopped (Ctrl+Z).\n");
            } else if (WIFEXITED(status)) {
                // Process exited normally
                int exit_status = WEXITSTATUS(status);
                printf("Process exited with status %d.\n", exit_status);
            } else if (WIFSIGNALED(status)) {
                // Process was terminated by a signal
                int signal_num = WTERMSIG(status);
                printf("Process terminated by signal %d.\n", signal_num);
            }
        }
    }
}
void check_background_processes(int *proc_no, pid_t *background_processes)
{
    int status;
    pid_t pid;

    for (int i = 0; i < (*proc_no); i++)
    {
        pid = waitpid(background_processes[i], &status, WNOHANG);
        if (pid > 0)
        {
            // A background process has finished
            if (WIFEXITED(status))
            {
                printf("\x1b[34mProcess with PID %d exited normally (%d)\x1b[0m\n", pid, WEXITSTATUS(status));

                // printf(BBLU"Process with PID %d exited normally (%d)\n"reset,pid, WEXITSTATUS(status));
            }
            else
            {
                printf("Background process with PID %d has ended.\n", pid);
            }
            // Remove the finished process from the bg_jobs array
            for (int j = i; j < (*proc_no) - 1; j++)
            {
                background_processes[j] = background_processes[j + 1];
            }
            (*proc_no)--;
            i--; // Decrement i because elements shifted left
        }
    }
}