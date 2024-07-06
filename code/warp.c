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

    // Handle different cases based on the argument
    if (strcmp(path, "-") == 0) {
        // Handle previous directory
        if (prev_dir[0] != '\0') {
            strcpy(new_dir, prev_dir);
        } else {
            printf("No previous directory stored.\n");
            return;
        }
    } else if (strcmp(path, "~") == 0) {
        // Handle home directory
        strcpy(new_dir, getenv("HOME"));
    } else if (strcmp(path, ".") == 0) {
        // Handle current directory (no change needed)
        snprintf(new_dir, sizeof(new_dir), "%s", current_dir);
        change_dir = 0;
    } else if (strcmp(path, "..") == 0) {
        // Handle parent directory
        strcpy(new_dir, current_dir);
        if (strcmp(new_dir, "/") != 0) {
            char *last_slash = strrchr(new_dir, '/');
            if (last_slash != NULL) {
                *last_slash = '\0'; // Remove the last part of the path
            }
        }
    } else {
        // Handle regular directory path
        snprintf(new_dir, sizeof(new_dir), "%s", path);
    }

    // Change directory using chdir if desired
    if (change_dir && chdir(new_dir) != 0) {
        perror("chdir");
        return;
    }

    // Store the current directory as the previous directory
    strcpy(prev_dir, current_dir);

    // Get the new working directory and print it
    if (getcwd(new_dir, sizeof(new_dir)) != NULL) {
        printf(" %s\n", new_dir);
    } else {
        perror("getcwd");
    }
}


