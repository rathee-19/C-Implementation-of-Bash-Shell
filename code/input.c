#include"headers.h"

void parse_args(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // Mark the end of the argument array
}


void execute_command(const char *command) {
    // Tokenize the command by spaces and tabs
    char *args[64]; // Adjust the size as needed
    int num_args = 0;

    char *token = strtok((char *)command, " \t");
    while (token != NULL && num_args < 64) {
        args[num_args++] = token;
        token = strtok(NULL, " \t");
    }

    if (num_args == 0) {
        return; // Empty command, nothing to execute
    }

    args[num_args] = NULL; // Null-terminate the argument list

    // Create a child process to execute the command
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return;
    }

    if (pid == 0) {
        // Child process
        execvp(args[0], args);
        perror("execvp"); // Exec failed
        exit(1);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    }
}