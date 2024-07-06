
#include "headers.h"
#include "bg.h"
int bg(int pid) {
  if (kill(pid, SIGCONT) < 0) {
    perror("Error");
    return -1;
  }

  return 0;
}