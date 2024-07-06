// seek.h

#ifndef SEEK_H
#define SEEK_H

 void seek(char *command, char *homedir); 
char* listFilesAndFolders(const char *basePath, const char *refer, int d, int f, int e, int none, char *homedir, int *count);

#endif // SEEK_H
