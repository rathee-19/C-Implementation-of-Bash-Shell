#include "headers.h"

void handle_redirection(const char *command) {
    // Check for input rediretion ("<")
    // printf("Hello redirection\n");
    // printf ("handle redirection %s\n", command);
    char *input_file = strchr(command, '<');
    if (input_file != NULL) {
        // Found the "<" operator, split the command into command and filename
        char *cmd_copy = strdup(command); // Create a copy of the command
        *input_file = '\0'; // Null-terminate the command
        input_file++; // Move to the next character (the filename)

        // Trim leading and trailing whitespace from the filename
        while (*input_file == ' ' || *input_file == '\t') {
            input_file++;
        }

        // Check for output redirection (">" or ">>") after input redirection
        char *output_file = strchr(input_file, '>');
        if (output_file != NULL) {
            // Found output redirection, handle it
            *output_file = '\0'; // Null-terminate the input filename
            output_file++; // Move to the next character (the output operator)

            // Trim leading and trailing whitespace from the output filename
            while (*output_file == ' ' || *output_file == '\t') {
                output_file++;
            }

            int input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1) {
                perror("Unable to open input file");
                free(cmd_copy);
                return;
            }

            int output_fd;
            if (strcmp(output_file, ">") == 0) {
                output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            } else if (strcmp(output_file, ">>") == 0) {
                output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            } else {
                perror("Invalid output redirection operator");
                free(cmd_copy);
                return;
            }

            if (output_fd == -1) {
                perror("Unable to open output file");
                free(cmd_copy);
                close(input_fd);
                return;
            }

            // Redirect standard input and output
            int saved_stdin = dup(STDIN_FILENO); // Save the current stdin
            int saved_stdout = dup(STDOUT_FILENO); // Save the current stdout

            dup2(input_fd, STDIN_FILENO); // Redirect stdin from the input file
            dup2(output_fd, STDOUT_FILENO); // Redirect stdout to the output file

            // Close file descriptors after redirection
            close(input_fd);
            close(output_fd);

            // Execute the command
            char *args[] = {"/bin/sh", "-c", cmd_copy, NULL}; // Use shell to run the command
            execvp(args[0], args);

            // Restore the original stdin and stdout
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);

            close(saved_stdin);
            close(saved_stdout);

            free(cmd_copy); // Free the memory allocated for the copy of the command
        } else {
            // No output redirection found after input redirection, handle input redirection only
            int input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1) {
                perror("Unable to open input file");
                free(cmd_copy);
                return;
            }

            // Redirect standard input
            int saved_stdin = dup(STDIN_FILENO); // Save the current stdin
            dup2(input_fd, STDIN_FILENO); // Redirect stdin from the input file
            close(input_fd); // Close the input file descriptor

            // Execute the command
            char *args[] = {"/bin/sh", "-c", cmd_copy, NULL}; // Use shell to run the command
            execvp(args[0], args);

            // Restore the original stdin
            dup2(saved_stdin, STDIN_FILENO);
            close(saved_stdin);

            free(cmd_copy); // Free the memory allocated for the copy of the command
        }
        
    }
}

// void handleinputoutputredirection(char **args)
// {
//     int originalout = 0;
//     int originalin = 0;

//     dup2(STDOUT_FILENO, originalout);
//     dup2(STDIN_FILENO, originalin);

//     int i = 1;
//     while (args[i] != NULL)
//     {
//         if (strcmp(args[i], "<") == 0)
//         {
//             int fd = open(args[i + 1], O_RDONLY);
//             dup2(fd, STDIN_FILENO);
//             // remove i and i+1 from args and keep args till that point

//             // NOW CHECK if THERE IS OUTPUT REDIRECTION
//             if (args[i + 2] != NULL && strcmp(args[i + 2], ">") == 0)
//             {
//                 int fd = open(args[i + 3], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//                 dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
//             }
//             else if (args[i + 2] != NULL && strcmp(args[i + 2], ">>") == 0)
//             {
//                 int fd = open(args[i + 3], O_WRONLY | O_APPEND);
//                 dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
//             }
//             args[i] = NULL;
//         }
//         else if (strcmp(args[i], ">") == 0)
//         {
//             int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
//             args[i] = NULL;
//         }
//         else if (strcmp(args[i], ">>") == 0)
//         {
//             int fd = open(args[i + 1], O_WRONLY | O_APPEND);
//             dup2(fd, STDOUT_FILENO); // STDOUT CLOSES
//             args[i] = NULL;
//         }
//         i++;
//     }

//     dup2(originalout, STDOUT_FILENO);
//     dup2(originalin, STDIN_FILENO);
// }
