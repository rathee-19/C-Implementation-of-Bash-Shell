#include "past.h"
#include "headers.h"

#define MAX_HISTORY_SIZE 15

int getHistory(char *root, char **commands)
{
    long unsigned size = 1024;
    char path[1024];
    int noOfCommands = 0;

    commands[0] = NULL;
    // strcpy(path, root);
    strcpy(path, "history.txt");

    FILE *file = fopen(path, "r");

    if (file)
    {
        for (int i = 0; i < MAX_HISTORY_SIZE; i++)
        {
            commands[i + 1] = NULL;
            if (getline(&commands[i], &size, file) > 0)
            {
                noOfCommands++;
            }
            else
            {
                break;
            }
        }
        fclose(file);
    }

    return noOfCommands;
}

void addHistory(char *str)
{
    char path[1024]; // Instead of path, use a local variable

    // Open the history file for appending
    // strcpy(path, root);
    strcpy(path, "history.txt");

    // printf("%s\n", path)
    FILE *file = fopen(path, "a");

    if (file)
    {

        // printf("helo");
        fprintf(file, "%s", str); // Write the new command to the file
        fclose(file);
    }
}

int pastevents(char *root, char *args)
{

    char *commands[MAX_HISTORY_SIZE + 1]; // Array of strings
    int noOfCommands = getHistory(root, commands);
    // printf("%d\n" , noOfCommands);

    if (noOfCommands > 0 && !strcmp(args, commands[noOfCommands - 1]))
    {
        // Do not store the same command as the most recent one
        return 0;
    }

    // printf("Hello\n");
    for (int i = 0; i < noOfCommands; i++)
        {
            // printf("hey");
        printf("%s", commands[i]);
        }
    return 0;
}
void purgeHistory()
{
    char path[1024];
    strcpy(path, "history.txt");

    // Open the history file for writing, which clears its content
    FILE *file = fopen(path, "w");

    if (file)
    {
        
        fclose(file);
        printf("Past events history purged.\n");
    }
    else
    {
        perror("Error purging past events history");
    }
}