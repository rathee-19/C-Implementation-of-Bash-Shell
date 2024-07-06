#include "headers.h"
#include "activities.h"
// #include"activities.h"

// Function to check if a string is numeric
// Function to check if a string is numeric
int isNumeric(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return 0;
        }
    }
    return 1;
}

// Function to list activities
void activities(){
 DIR *dir;
    struct dirent *entry;
    
    // Open the /proc directory
    dir = opendir("/proc");
    if (dir == NULL) {
        perror("Unable to open /proc directory");
        exit(1);
    }

    printf("Command Name\tPID\tState\n");
    
    // Loop through each entry in the /proc directory
    while ((entry = readdir(dir)) != NULL) {
        char proc_path[256];
        FILE *stat_file;
        char command[256];
        int pid;
        char state;

        // Check if the entry is a directory and represents a process
        if (sscanf(entry->d_name, "%d", &pid) != 1) {
            continue;
        }

        // Build the path to the /proc/[PID]/stat file
        snprintf(proc_path, sizeof(proc_path), "/proc/%d/stat", pid);
        
        // Open the /proc/[PID]/stat file
        stat_file = fopen(proc_path, "r");
        if (stat_file == NULL) {
            continue;
        }
        
        // Read process information from the stat file
        if (fscanf(stat_file, "%d %s %c", &pid, command, &state) == 3) {
            // Filter processes spawned by your shell (modify as needed)
             
                printf("%d : %s - %s\n", pid, command, (state == 'R') ? "Running" : "Stopped");
            
        }

        fclose(stat_file);
    }
    
    closedir(dir);
    
    }

int active(int* bgPid, char** bgCommand) {
//     printf("comming___jobs");
//     printf("id---%d",bgPid);
//   printf("name--%s",bgCommand[1]);
  char fileName[1024];
  char status;
  int retValue = 0;

  for (int i = 1; i < 100; i++) {
    // printf("value_at%d---->%d\n\n",i,bgPid[i]);
    if (!bgPid[i]) continue;

    sprintf(fileName, "/proc/%d/stat", bgPid[i]);

    FILE* file = fopen(fileName, "r");
    // printf("file--%s\n",fileName);

    if (file) {
      fscanf(file, "%*d %*s %c", &status);

      printf("[%d] %s %s [%d]\n", i, status == 'T' ? "Stopped" : "Running",
             bgCommand[i], bgPid[i]);

    } else {
      perror("Error");
      retValue = -1;
    }

    fclose(file);
  }

  return retValue;
}