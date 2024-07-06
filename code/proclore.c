#include "headers.h"

void print_process_status(char status) {
    if (status == 'R') {
        printf("Running\n");
    } else if (status == 'S') {
        printf("Sleeping in an interruptible wait\n");
    } else if (status == 'Z') {
        printf("Zombie\n");
    }
}

void proclore(char *args) {
    int pid;
    printf("%s",args);

    if (args == NULL) {
        // Print information of your shell (current process)
        pid = getpid();
    } else {
        // Parse the argument as PID
        pid = atoi(args);
    }

    // Get process status using stat file
    char status;
    char status_path[1024];
    snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);

    FILE *status_file = fopen(status_path, "r");
    if (status_file) {
        while (fgets(status_path, sizeof(status_path), status_file)) {
            if (strncmp(status_path, "State:", 6) == 0) {
                sscanf(status_path, "State:\t%c", &status);
                break;
            }
        }
        fclose(status_file);
    }

    // Get process group and virtual memory size using statm file
    int process_group;
    unsigned long virtual_memory;
    char statm_path[1024];
    snprintf(statm_path, sizeof(statm_path), "/proc/%d/statm", pid);

    FILE *statm_file = fopen(statm_path, "r");
    if (statm_file) {
        fscanf(statm_file, "%d %lu", &process_group, &virtual_memory);
        fclose(statm_file);
    }

    // Get executable path using exe symlink
    char executable_path[1024];
    char exe_path[1024];
    snprintf(exe_path, sizeof(exe_path), "/proc/%d/exe", pid);
    ssize_t len = readlink(exe_path, executable_path, sizeof(executable_path) - 1);
    if (len != -1) {
        executable_path[len] = '\0';
    } else {
        strcpy(executable_path, "Not available");
    }

    // Print the obtained information
    printf("pid : %d\n", pid);
    printf("process status : %c", status);
    if (status == 'R' || status == 'S') {
        char foreground = (pid == getpgid(pid)) ? '+' : ' ';
        printf("%c\n", foreground);
    } else {
        printf("\n");
    }
    printf("Process Group : %d\n", process_group);
    printf("Virtual memory : %lu\n", virtual_memory);
    printf("executable path : %s\n", executable_path);

    // if (status == 'R' || status == 'S' || status == 'Z') {
    //     printf("Process states :\n");
    //     printf("R/R+ : Running\n");
    //     printf("S/S+ : Sleeping in an interruptible wait\n");
    //     printf("Z : Zombie\n");
    // }
}
