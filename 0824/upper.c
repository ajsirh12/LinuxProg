#include<stdio.h>
#include<ctype.h>
#include<stdlib.h>
#include<unistd.h>

#define STR_SIZE 100

int main(){
	int ch;
	char *temp;

	temp = malloc(STR_SIZE);
	
	while((ch = getchar()) != EOF){
		putchar(toupper(ch));
	}
	exit(0);
}
