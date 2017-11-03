#include "polling.h"


static int val=3;


int main(){
    val++;
    int pid;
    for (int i = 0; i < 3; i++) {
      pid = fork();
      if (pid == 0){
        val ++;
        printf("i= %d #%d \n",i,val);
        exit(0);
      }
      else {
        val++;
        printf("%d \n", val);
      }
    }
    return EXIT_SUCCESS;
}
