#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int count = 0;

static void sigHandler3(int sig){
    count++;

    printf("sigHandler : %d\n", count);
}

int main(int argc, char* argv[]){
    int i, status;
	int sig, result;
	pid_t pid;
	
	pid = fork();

    //정상적으로 자식프로세서 생성되지 않은 경우
    if(pid == -1){
        printf("Error: fork()\n");
        exit(-1);
    }
    //부모프로세스가 실행될 구문
    else if(pid > 0){
        for(i=0;i<10;i++){
            result = kill(pid, SIGINT);
            //printf("%d : %d\n", i, pid);
            //signal이 정상적으로 전송되지 않은 경우
            if(result == -1){
                printf("Error: kill()\n");
                exit(-2);
            }
        }
        //wait(&status);
        exit(0);
    }
    //자식프로세스가 실행될 구문
    else{
        signal(SIGINT, sigHandler3);
        for(i=0;i<10;i++){
            pause();
            printf("Hello\n");
        }
        exit(0);
    }
        
	return 0;
}

