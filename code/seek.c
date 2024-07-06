#include "headers.h" 
#include "seek.h"
  
 void seek(char *command, char *homedir) 
 { 
    printf("%sheylll",command);
     int d = 0; 
     int e = 0; 
     int f = 0; 
  
     for (int i = 0; i < strlen(command); i++) 
     { 
         if (command[i - 1] == '-') 
         { 
             if (command[i] == 'd') 
                 d = 1; 
             if (command[i] == 'e') 
                 e = 1; 
             if (command[i] == 'f') 
                 f = 1; 
         } 
     } 
  
     // printf("%d %d %d \n", d, f, e); 
    printf("entered\n");
  
     if (d * f == 1) 
     { 
         printf("Invalid Flag\n"); 
         return; 
     } 
  
     char search[1000] = {'\0'}; 
     char target[1000] = {'\0'}; 
     char *token; 
     char command2[1000]; 
     strcpy(command2, command); 
     token = strtok(command2, " "); 
     while (token != NULL) 
     { 
         // printf("hey hel"); 
         strcpy(search, target); 
         strcpy(target, token); 
         token = strtok(NULL, " "); 
     } 

  
     int count = 0; 
     char* string; 
  
     if (strncmp(search, "seek", 4) == 0 || strncmp(search, "-", 1) == 0) 
     { 
         strcpy(search, target); 
         strcpy(target, homedir); 
        //  printf("%s ____ %s", target, search);
         printf("\n\nContents of folder: %s %s[%ld]\n", target, search, strlen(search)); 
         string = listFilesAndFolders(target, search, d, f, e, d + e + f, homedir, &count); 
     } 
     else 
     { 
         printf("\n\nContents of folder: %s %s[%ld]\n", target, search, strlen(search)); 
        //  printf("\n\nContents of folder: %s %s\n", search, target); 
    //  printf("---%s : %s\n", target, search);
         string = listFilesAndFolders(target, search , d, f, e, d + e + f, homedir, &count); 
     } 
  
     // printf("%s %s\n\n", search, target); 
     if (count == 0) 
     { 
         printf("NO MATCH FOUND\n"); 
     } 
     if(count ==1 && e==1) 
     { 
         if(f==1) 
         { 
             // print contents; 
             // printf("%s\n\n", string); 
             int fd = open(string, O_RDONLY); 
             char BUFFER[1000]; 
             read(fd, BUFFER, sizeof(BUFFER)); 
             printf("%s", BUFFER); 
             close(fd); 
             printf("\n"); 
         } 
         if(d==1) 
         { 
             string[strlen(string)] = '\0'; 
             // printf("%d\n\n", chdir(string)); 
         // printf("YES\n"); 
         // printf("%s\n", string); 
         // printf("%c\n\n", ) 
         } 
     } 
 } 
  
 char* listFilesAndFolders(const char *basePath, const char *refer, int d, int f, int e, int none, char *homedir, int *count) 
 { 
     char currdir[1024]; 
     char storer[1024]; 
     getcwd(currdir, sizeof(currdir)); 
     struct dirent *dp; 
     struct stat st; 
     char path[1024]; 
  
     DIR *dir = opendir(basePath); 
  
     // Check if the directory exists 
  
     // printf("%s : %ld\n\n", basePath, strlen(basePath)); 
     if (!dir) 
     { 
         perror("Unable to open directory"); 
         return NULL; 
     } 
  
     const char *blueColor = "\033[34m"; 
     const char *greenColor = "\033[32m"; 
     const char *resetColor = "\033[0m"; 
  
     // Loop through all entries in the directory 
     while ((dp = readdir(dir)) != NULL) 
     { 
         // Skip "." and ".." special entries 
         if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) 
         { 
             snprintf(path, sizeof(path), "%s/%s", basePath, dp->d_name); 
  
             // Get file/directory information 
             if (stat(path, &st) == 0) 
             { 
                 if (S_ISDIR(st.st_mode)) 
                 { 
                     // It's a directory, recursively list its contentsin 
                     int flag = 0, c = 0; 
                     for (int i = 0; i <= strlen(refer); i++) 
                     { 
                         if (refer[strlen(refer) - i] == path[strlen(path) - i]) 
                         { 
                             flag++; 
                         } 
                         c++; 
                     } 
                     if (flag == c && ((none == 0 || d == 1))) 
                     { 
                         printf("%sDirectory: %s%s\n", blueColor, path, resetColor); 
                         strcpy(storer, path); 
                         *count+=1; 
                     } 
                     listFilesAndFolders(path, refer, d, f, e, none, homedir, count); 
                 } 
                 else if ((none == 0 || f == 1)) 
                 { 
                    // printf("Came\n");
                     // It's a file, print its path 
                     int pos = 0; 
                     for (int i = 0; i < 6; i++) 
                     { 
                         if (path[strlen(path) - i] == '.') 
                         { 
                             pos = i; 
                             // printf("%d\n", pos); 
                             break; 
                         } 
                     } 
                     int flag = 0, c = 0; 
                     
                     if(strstr(path, refer) != NULL)
                     {
                        char* new = strstr(path, refer);
                        
                        if(new[strlen(refer)] == '.')
                        {
                            flag = 1;
                        }
                     }

                    //  printf("%s ::::: %s \n", refer, path);
                     // printf("\n"); 
                     if (flag == 1) 
                     { 
                         printf("%sFile: %s%s\n", greenColor, path, resetColor); 
                         strcpy(storer, path); 
                         *count+=1; 
                     } 
                 } 
             } 
            //  else 
            //  { 
            //      perror("Unable to get file/directory info"); 
            //  } 
         } 
     } 
  
     // Close the directory 
     closedir(dir); 
     if(e==1 && d==1 && *count == 1) 
     { 
         for(int i=1; i<= strlen(storer); i++) 
         { 
             storer[i-1] = storer[i]; 
         } 
         strcat(homedir, storer); 
         return homedir; 
     } 
     else if(e==1 && f==1 && *count == 1) 
     { 
         for(int i=1; i<= strlen(storer); i++) 
         { 
             storer[i-1] = storer[i]; 
         } 
         strcat(homedir, storer); 
         return homedir; 
     } 
     return NULL; 
 }