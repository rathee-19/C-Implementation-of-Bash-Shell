#ifndef __PROMPT_H
#define __PROMPT_H

extern char* username;
extern char* systemname;
extern char initdir[256];
extern char prevdir[256];
extern int shellpid;

void prompt();
void initialise();

#endif