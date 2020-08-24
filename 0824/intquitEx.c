#include<stdio.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>

int main(){

	pid_t pid;
	pid = fork();
	while(1){
		if(pid == 0){
			execl("./intquitEx2", "intquitEx2", NULL);
			exit(1);
		}
		else{
			kill(pid, SIGINT);
		}
	}
	
	return 0;
}
