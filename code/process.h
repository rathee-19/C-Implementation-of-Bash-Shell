#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_BACKGROUND_PROCESSES 50
extern pid_t foregroundProcessPid;

void execute_process(int num_args, char **args, int background_flag, int *proc_no, pid_t *background_processes, char **backgroundCommand, int *backgroundprocessesid);
void check_background_processes(int *proc_no, pid_t *background_processes);

#endif
