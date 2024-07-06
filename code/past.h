#ifndef _HISTORY_H
#define _HISTORY_H

typedef struct queuenode {
    char *command; // name of command
    struct queuenode* nextnode; // pointer to nextnode
}Queuenode;

typedef struct queue{
    Queuenode* head; // head of queue
    Queuenode* rear; // rear of queue
    int size;
    int capacity; // capacity of queue , by default 20
}Queue;

Queue* createqueue();
Queuenode* createqueuenode(char *command);
int enqueue(char * command,Queue* que);
void dequeue(Queue* que);
void history(Queue* que);
int isfull(Queue* que);


#endif // !1