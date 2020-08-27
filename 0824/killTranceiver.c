#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

//./killTranceiver pid signalNumber

int main(int argc, char* argv[]){
	int sig, result;
	pid_t pid;
	
	pid = atoi(argv[1]);
	printf("pid : %d\n", pid);
	
	sig = atoi(argv[2]);
	printf("signal : %d\n", sig);

	result = kill(pid, sig);
	
	//signal이 정상적으로 전송되지 않은 경우
	if(result == -1){
		printf("Error: kill()\n");
		exit(-1);
	}

	return 0;
}

