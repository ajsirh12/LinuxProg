#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

static void sigHandler(int sig){
    static int count;
    count = 0;
    count++;

    printf("sigHandler : %d\n", count);
}

int main(int argc, char* argv[]){
    
    signal(SIGINT, sigHandler);
    for(int i=0;i<10;i++){
        pause();
        printf("Hello\n");
    }
    
	exit(0);
}

