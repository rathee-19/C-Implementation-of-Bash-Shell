#include "headers.h"

#define MAX_BACKGROUND_PROCESSES 50
int backgroundprocessesid[1000];
int foregroundprocessid = -1;
char *backgroundCommand[100];
pid_t foregroundProcessPid = 0;

int inpre = 1;
int outre = 1;
int proc_no = 0;
int error = 0;
char *username = NULL;
char my_systemname[1024];

int foregroundRunning = 0;
// Initialize the number of background processes
pid_t background_processes[MAX_BACKGROUND_PROCESSES]; // Initialize an array to store background process PIDs

void removeBg(int pid)
{
    if (!pid)
        return;

    for (int i = 1; i < 100; i++)
    {
        if (backgroundprocessesid[i] == pid)
        {
            backgroundprocessesid[i] = 0;
            free(background_processes[i]);
            break;
        }
    }
}

#include "headers.h"

void bgHandler()
{
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0)
    {
        int pos = 0;
        while (backgroundprocessesid[pos] != pid)
            pos++;

        if (WIFSTOPPED(status))
            return;
        printf("\n");
        fprintf(stderr, "%s with PID %d exited %s\n", backgroundCommand[pos], backgroundprocessesid[pos],
                WIFEXITED(status) ? "normally" : "abnormally");

        // printf("\n");
        printf("coming signal here also !!!!\n");

        prompt(username, my_systemname);
        fflush(stdout);

        backgroundprocessesid[pos] = 0;
        free(backgroundCommand[pos]);
    }
}
// void ctrlcHandler(int sig) {
//     printf("\n%s", makePrompt());
//     fflush(stdout);
// }

// void ctrlzHandler(int sig) {
//     printf("\n%s", makePrompt());
//     fflush(stdout);
// }

// void ctrlCZhandler()
// {
//     // printf("%d",foregroundRunning);
//     if (foregroundProcessPid > 0)
//     {
//         printf("\n");
//         // printf("user--> %s\n system --->%s",username,my_systemname);
//         // printf("\n");
//         printf("coming signal\n");
//         kill(foregroundProcessPid, SIGINT);

//         // prompt(username, my_systemname);
//         // fflush(stdout); // Flush the output buffer to ensure prompt is displayed.

//         // printf("after  prompt\n");
//     }
//     else
//     {
//         printf("\n");
//         prompt(username, my_systemname);
//         fflush(stdout); // Flush the output buffer to ensure prompt is displayed.

//         // foregroundRunning = 0;
//     }
// }
void ctrlCZhandler(int signum)
{

    if (signum == SIGINT)
    {
        printf("foreground in the handler function %d", foregroundProcessPid);
        // Ctrl+C (SIGINT) was pressed
        if (foregroundProcessPid > 0)
        {
            printf("\nCtrl+C received: Sending SIGINT to PID %d\n", foregroundProcessPid);
            kill(foregroundProcessPid, SIGINT);
        }
    }
    else if (signum == SIGTSTP)
    {
        // Ctrl+Z (SIGTSTP) was pressed
        if (foregroundProcessPid > 0)
        {
            printf("\nCtrl+Z received: Stopping PID %d\n", foregroundProcessPid);
            kill(foregroundProcessPid, SIGTSTP);
        }
    }
    else
    {
        // Handle other signals here if needed
    }
}
int main()
{
    time_t totaltimespent = 0;
    time_t begin = 0, end = 0;
    char *homedir = getenv("HOME");

    // char *username = (char *)calloc(256, sizeof(char));
    username = getenv("USER");

    // char *my_systemname = (char *)calloc(256, sizeof(char));
    gethostname(my_systemname, 1024);
    char root[1024];                                         // Set the path to store history
    snprintf(root, sizeof(root), "/path/to/history/folder"); // Replace with actual path
    int job_number = 1;
    // printf("user--> %s\n system --->%s",username,my_systemname);

    signal(SIGCHLD, bgHandler);
    signal(SIGINT, ctrlCZhandler);
    signal(SIGTSTP, ctrlCZhandler);
    // <-------------------------------------------------------------------------------------__>
    while (1)
    {
        if (feof(stdin))
        {
            printf("ctrlD check");
            printf("\n"); // Print a newline for a clean prompt
            break;        // Exit the loop
        }

        // Print appropriate prompt with username, my_systemname, and directory before accepting input
        prompt(username, my_systemname);

        char input[4096];
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        char c[1000];
        strcpy(c, input);

        // Tokenize the input by semicolon to separate commands
        char *commands[32]; // Adjust the array size as needed
        int num_commands = 0;

        char *token = strtok(input, ";");
        // printf("token %s\n" , token);
        while (token != NULL)
        {
            commands[num_commands] = token;
            num_commands++;
            token = strtok(NULL, ";");
        }

        // printf("%s\n" , commands[0]);
        for (int i = 0; i < num_commands; i++)
        {
            char *current_command = commands[i];
            int background_flag = 0;
            int cmd_length = strlen(commands[i]);
            if (cmd_length > 0 && commands[i][cmd_length - 1] == '&')
            {

                // printf("------>%s\n",commands[i][cmd_length - 1]);
                // printf("helooo");
                background_flag = 1;
                // int backgroundID = pid_t
                commands[i][cmd_length - 1] = '\0';
                // job_number++;
            }
            // printf("crr---%s\n",current_command);
            // printf("before redirection %s\n" , current_command);

            // printf("%s",current_command);
            handle_redirection(current_command);
            // printf("kfnv");

            if (strncmp(current_command, "pastevents", 10) != 0)
                addHistory(current_command);

            current_command[strcspn(current_command, "\n")] = '\0';
            // printf("current commadn %s\n" , current_command);

            // Tokenize the current command to extract command and arguments
            char *cmd_token = strtok(current_command, " ");
            // printf("cmd %s\n" , cmd_token);
            if (cmd_token == NULL)
            {
                continue; // No command provided, go back to the prompt
            }
            // printf("token ----->%s",current_command[1]);
            if (strcmp(cmd_token, "warp") == 0)
            {
                // Handle the warp command and its arguments
                cmd_token = strtok(NULL, " "); // Get the next token (argument)
                while (cmd_token != NULL)
                {
                    // Execute the warp command with the current argument
                    warp(cmd_token);

                    // Get the next argument
                    cmd_token = strtok(NULL, " ");
                }
            }
            else if (strcmp(cmd_token, "peek") == 0)
            {

                // Handle the peek command and its arguments
                cmd_token = strtok(NULL, " "); // Get the next token (argument)
                if (cmd_token != NULL)
                {
                    int show_hidden = 0;
                    int show_details = 0;

                    // Parse flags
                    while (cmd_token[0] == '-')
                    {
                        if (strcmp(cmd_token, "-a") == 0)
                        {
                            show_hidden = 1;
                        }
                        else if (strcmp(token, "-l") == 0)
                        {
                            show_details = 1;
                        }

                        // Get the next token (flag)
                        cmd_token = strtok(NULL, " ");
                    }

                    // Execute the peek command with the current argument and flags
                    peek(cmd_token, show_hidden, show_details);
                }
                else
                {
                    // No path provided, peek at the current directory without flags
                    peek(".", 0, 0);
                }
            }
            // Add more commands here if needed
            else if (strcmp(cmd_token, "pastevents") == 0)
            {
                // Handle the pastevents command and its arguments
                // token = strtok(NULL, ""); // Get the rest of the input as argument

                if (strlen(cmd_token) >= 10 && strncmp(&cmd_token[11], "purge", 5) == 0)
                {

                    purgeHistory();
                }
                else if (strlen(cmd_token) >= 10 && strncmp(&cmd_token[11], "execute", 7) == 0)
                {

                    purgeHistory();
                }
                else
                {
                    pastevents(root, cmd_token);
                }
            }
            else if (strcmp(cmd_token, "proclore") == 0)
            {
                // Handle the proclore command and its arguments
                cmd_token = strtok(NULL, ""); // Get the rest of the input as argument
                proclore(cmd_token);          // Use the proclore function
            }

            else if (strcmp(cmd_token, "seek") == 0)
            {
                printf("%s", c);
                // Handle the seek command and its arguments
                cmd_token = strtok(NULL, " "); // Get the next token (argument)
                char *command = c;

                // The second argument is homedir, which you may need to define or obtain elsewhere in your main function.

                // Call the seek function with the provided arguments
                printf("%s", homedir);
                seek(command, homedir);
            }
            else if (strcmp(cmd_token, "activities") == 0)
            {
                printf("before going to activity %d\n\n", backgroundprocessesid[1]);
                if (active(backgroundprocessesid, backgroundCommand) < 0)
                    error = 1;

                // printProcessList();
            }

            else if (strcmp(cmd_token, "ping") == 0)
            {
                // printf("------------------going &\n");
                char *command = strtok(NULL, " ");
                char *args[32];
                args[0] = cmd_token;
                int num_args = 1;

                // printf("cmd %s\n" , cmd_token);
                // printf("arg1 %s\n" , command);

                while (command != NULL)
                {
                    args[num_args] = command;
                    num_args++;
                    command = strtok(NULL, " ");
                }

                // printf("args 1 %s\n" , args[0]);
                args[num_args] = NULL;

                if (num_args < 3)
                {
                    error = 1;
                    printf("Error: Too few arguments\n");
                }
                else
                {

                    pid_t targetPid = 0;
                    int sig = 0;
                    sscanf(args[1], "%d", &targetPid);
                    sscanf(args[2], "%d", &sig);
                    // printf("job__id-->%d", backgroundprocessesid[jobNo]);
                    // printf("job___no---->%d", job_number);

                    if (targetPid <= 1)
                    {
                        error = 1;
                        printf("Error: Invalid Job\n");
                    }
                    else if (kill_job(targetPid, sig) < 0)
                        error = 1;
                }
            }

            else if (strcmp(cmd_token, "fg") == 0)
            {
                // Tokenize the input again for the command and arguments
                char *command = strtok(NULL, " ");
                char *args[32];
                args[0] = cmd_token;
                int num_args = 1;

                // Tokenize the remaining input into arguments
                while (command != NULL)
                {
                    args[num_args] = command;
                    num_args++;
                    command = strtok(NULL, " ");
                }

                args[num_args] = NULL; // Null-terminate the argument list

                // Now you have the number of arguments in 'num_args' and the arguments in the 'args' array
                // You can use 'num_args' to check the number of arguments and 'args' to access individual arguments

                // printf("args----%d\n",num_args);
                if (num_args < 2)
                {
                    error = 1;
                    printf("Error: Too few arguments\n");
                }
                else
                {
                    int jobNo = 0;
                    sscanf(args[1], "%d", &jobNo);
                    // printf("check\n");
                    // printf("checking---%s\n",background_processes);

                    if (jobNo < 1 || jobNo > 99 || background_processes[jobNo] <= 1)
                    {

                        // printf("job---%d\n",jobNo);
                        printf("Error: Invalid Job\n");
                    }
                    else
                    {
                        int pid = fg(background_processes[jobNo]);

                        removeBg(pid > 0 ? pid : -pid);

                        if (pid <= 0)
                            error = 1;
                    }
                }
            }
            else if (strcmp(cmd_token, "bg") == 0)
            {
                char *command = strtok(NULL, " ");
                char *args[32];
                args[0] = cmd_token;
                int num_args = 1;

                // Tokenize the remaining input into arguments
                while (command != NULL)
                {
                    args[num_args] = command;
                    num_args++;
                    command = strtok(NULL, " ");
                }

                args[num_args] = NULL;
                if (num_args < 2)
                {
                    error = 1;
                    printf("Error: Too few arguments\n");
                }
                else
                {
                    int jobNo = 0;
                    sscanf(args[1], "%d", &jobNo);

                    if (jobNo < 1 || jobNo > 99 || backgroundprocessesid[jobNo] <= 1)
                    {
                        printf("Error: Invalid Job\n");
                    }
                    else
                    {
                        if (bg(backgroundprocessesid[jobNo]) < 0)
                            error = 1;
                    }
                }
            }
            else if (strcmp(cmd_token, "quit") == 0)
            {
                exit(0);
            }
            else if (strcmp(cmd_token, "neonate") == 0)
            {
            }
            else
            {
                // printf("%s",cmd_token);
                // Tokenize the input again for the command and arguments
                // printf("------------------going &\n");
                char *command = strtok(NULL, " ");
                char *args[32];
                args[0] = cmd_token;
                int num_args = 1;

                // printf("cmd %s\n" , cmd_token);
                // printf("arg1 %s\n" , command);

                while (command != NULL)
                {
                    args[num_args] = command;
                    num_args++;
                    command = strtok(NULL, " ");
                }

                // printf("args 1 %s\n" , args[0]);
                args[num_args] = NULL; // Null-terminate the argument list
                // printf("%s\n" , command);
                // Execute the command in foreground or background based on the flag
                foregroundRunning = 1;
                execute_process(num_args, args, background_flag, &proc_no, background_processes, backgroundCommand, backgroundprocessesid);
                // printf("checking___->%s\n",args[0]);
                // addBg(proc_no, args[0],&proc_no);
                foregroundRunning = 0;
            }
            // printf("proc---%d", proc_no);
            check_background_processes(&proc_no, background_processes);
        }
    }

    // Free allocated memory
    free(username);
    free(my_systemname);

    return 0;
}
