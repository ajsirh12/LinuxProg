#include<signal.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

static void sigHandler(int sig){
	static int count = 0;
	count++;
	
	if(sig == SIGINT)
		printf("Caught SIGINT : %d\n", count);
	else if(sig == SIGQUIT)
		printf("Caught SIGQUIT : %d\n", count);
}

int main(int argc, char* argv[]){
	//signal handler 등록
	if(signal(SIGINT, sigHandler) == SIG_ERR){
		printf("Error : signal(SIGINT)\n");
		exit(-1);
	}
	
	if(signal(SIGQUIT, sigHandler) == SIG_ERR){
		printf("Error : signal(SIGQUIT)\n");
		exit(-2);
	}
	
	while(1){
		pause();
		printf("Hello\n");
	}
		
	return 0;
}
