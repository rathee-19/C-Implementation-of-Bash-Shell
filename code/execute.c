#include "headers.h"

int inpre = 1;
int outre = 1;

int execute(char *command, char **args, int background) // THIS GETS CALLED IN CHILD PROCESS !!!!!!
{
    int originalout = 0;
    int originalin = 0;

    if (outre)
        dup2(STDOUT_FILENO, originalout);
    if (inpre)
        dup2(STDIN_FILENO, originalin);

    int i = 1;
    int fd;
    while (args[i] != NULL)
    {
        if (inpre && (strcmp(args[i], "<") == 0))
        {
            fd = open(args[i + 1], O_RDONLY);
            dup2(fd, STDIN_FILENO);
            close(fd);
            // remove i and i+1 from args and keep args till that point

            // NOW CHECK if THERE IS OUTPUT REDIRECTION
            if (outre && args[i + 2] != NULL && strcmp(args[i + 2], ">") == 0)
            {
                fd = open(args[i + 3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
                close(fd);
            }
            else if (outre && args[i + 2] != NULL && strcmp(args[i + 2], ">>") == 0)
            {
                fd = open(args[i + 3], O_WRONLY | O_APPEND);
                dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
                close(fd);
            }
            args[i] = NULL;
            break;
        }
        else if (outre && (strcmp(args[i], ">") == 0))
        {
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
            args[i] = NULL;
            close(fd);
            break;
        }
        else if (outre && strcmp(args[i], ">>") == 0)
        {
            fd = open(args[i + 1], O_WRONLY | O_APPEND);
            dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
            args[i] = NULL;
            close(fd);
            break;
        }
        i++;
    }

    if (strcmp(command, "ls") == 0)
        ls(args);

    else if (strcmp(command, "pinfo") == 0)
        pinfo(args);

    else if (strcmp(command, "discover") == 0)
        discover(args);

    else if (strcmp(command, "jobs") == 0)
        jobs(backgroundprocesses, backgroundprocessesid, numberofbackgroundprocess, args);

    else if (strcmp(command, "sig") == 0)
    {
        int processno = -1;
        int signalnumber = -1;

        processno = atoi(args[1]);
        signalnumber = atoi(args[2]);

        sig(backgroundprocessesid, numberofbackgroundprocess, processno, signalnumber);
    }
    else if (strcmp(command, "bg") == 0)
    {
        int backgroundprocessno = atoi(args[1]);
        bg(backgroundprocessesid, numberofbackgroundprocess, backgroundprocessno);
    }
    else if (strcmp(command, "fg") == 0)
    {
        int backgroundprocessno = atoi(args[1]);

        begin = 0;
        end = 0;
        begin = time(NULL);
        fg(backgroundprocessesid, numberofbackgroundprocess, backgroundprocessno);
        end = time(NULL);
        totaltimespent += end - begin;
    }
    else // FOR EXECVP TYPE COMMANDS
    {
        int forkret = fork();

        // IN PARENT
        if (forkret > 0)
        {
            if (background)
                return forkret; // NO NEED TO WAIT
            else                // if process is to be run in foreground
            {
                foregroundprocessid = forkret;
                strcpy(foregroundprocessname, args[0]);

                signal(SIGTTIN, SIG_IGN);
                signal(SIGTTOU, SIG_IGN); // IGNORING CERTAIN SIGNALS

                tcsetpgrp(0, forkret); // giving foreground control to child process

                begin = 0;
                end = 0;
                begin = time(NULL);
                waitpid(forkret, &status_, WUNTRACED);
                end = time(NULL);
                totaltimespent += end - begin;

                tcsetpgrp(0, getpgid(0)); // REGAINING CONTROL OF FOREGROUND

                signal(SIGTTIN, SIG_DFL);
                signal(SIGTTOU, SIG_DFL); // ACTIVATING CERTAIN SIGNALS

                if (outre)
                    dup2(originalout, STDOUT_FILENO);
                if (inpre)
                    dup2(originalin, STDIN_FILENO);

                return forkret; // DOESN'T MATTER WHAT WE RETURN
            }
        }
        else
        {
            setpgid(0, 0);                 // setting process group id of child to its process id
            if (execvp(args[0], args) < 0) // child will exit after this
            {
                perror("Error");
                exit(1);
            }
        }
    }

    if (outre)
        dup2(originalout, STDOUT_FILENO);
    if (inpre)
        dup2(originalin, STDIN_FILENO);

    return 0;
}
