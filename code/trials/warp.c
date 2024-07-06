#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_PATH_LENGTH 4096 // Define a maximum path length

   char prev_dir[MAX_PATH_LENGTH] = "";
void warp(const char *path) {
    char current_dir[MAX_PATH_LENGTH];

    char new_dir[MAX_PATH_LENGTH];
        int change_dir = 1; // Variable to control directory change

    
    // Get the current working directory
    if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
        perror("getcwd");
        return;
    }
    // strcpy(prev_dir, current_dir);
    //         printf("%s", prev_dir);

    //  printf("%d", path[0]);
    // Handle different cases based on the argument
    // if (path[0] == '~') {
    //     // Handle home directory
    //          printf(" %d\n", path[0]);
    //       printf("%s\n", getenv("HOME"));
    //                 printf("%s\n", path +1);

    //     snprintf(new_dir, MAX_PATH_LENGTH, "%s%s", getenv("HOME"), path + 1);

    // } else
     if (strcmp(path, "-") == 0) {
        // Handle previous directory
        // strcpy(new_dir, prev_dir);
        // printf("%s", prev_dir);
        if (prev_dir[0] != '\0') {
            strcpy(new_dir, prev_dir);
            printf("%s\n\n",new_dir);
        // snprintf(new_dir, sizeof(new_dir), "%s", current_dir);
        chdir("new_dir");

        } else {
            printf("No previous directory stored.\n");
            return;
        }

        
    } 
    else if (strcmp(path, "~") == 0) {
        // Handle home directory
        strcpy(new_dir, getenv("HOME"));
        // printf("%s", getenv("HOME"));
        
        // Change directory to the home directory
        if (chdir(new_dir) != 0) {
            perror("chdir");
            return;
        }
    }
    else if (strcmp(path, ".") == 0) {
        // Handle current directory (no change needed)
        snprintf(new_dir, sizeof(new_dir), "%s", current_dir);
        change_dir = 0;
    } else if (strcmp(path, "..") == 0) {
        // Handle parent directory
        snprintf(new_dir, sizeof(new_dir), "%s", current_dir);
        chdir("..");
        // printf("%s");
    } else {
        // Handle regular directory path
        snprintf(new_dir, sizeof(new_dir), "%s", path);
        chdir(new_dir);
    }
     if (change_dir && chdir(new_dir) != 0) {
        perror("chdir");
        return;
    }

    // Get the new working directory and print it
     strcpy(prev_dir, current_dir);
            printf("%s", prev_dir);
    if (getcwd(new_dir, sizeof(new_dir)) != NULL) {
        printf("New working directory: %s\n", new_dir);
    } else {
        perror("getcwd");
    }
}

