#include<signal.h>
#include<stdio.h>
#include<unistd.h>

// SIGINT에 대응하는 핸들러
void sigHandler(int sig){
	printf("OUCH! - I got signal %d\n", sig);
	//signal handeler 재등록
	(void)signal(SIGINT, SIG_DFL);
}

int main(){
	signal(SIGINT, sigHandler);
	
	while(1){
		printf("Hello World!\n");
		sleep(1);
	}
	
	return 0;
}
