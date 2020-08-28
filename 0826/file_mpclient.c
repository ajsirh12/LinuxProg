#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 8096
#define FILEPATH "./receive/"
#define DDN "dn "
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char message[BUF_SIZE];
	int str_len;
	struct sockaddr_in serv_adr;

    char filepath[100] = "./receive/";
    char* filename;
    FILE* fp;
    int read_cnt;

	if(argc!=3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	sock=socket(PF_INET, SOCK_STREAM, 0);   
	if(sock==-1)
		error_handling("socket() error");
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));
	
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected...........");
	
	while(1) 
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if(!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

        else if(!strncmp(message, DDN, 3)){
            write(sock, message, strlen(message));

            filename = strtok(message, " ");
            filename = strtok(NULL, " ");
            filename[strlen(filename)-1] = 0;

            /* str_len=read(sock, message, BUF_SIZE-1);
            message[str_len]=0; */
        
            char* filefullpath = strcat(filepath, filename);
            fp=fopen(filefullpath, "wb"); 
            
            /* while((read_cnt=read(sock, message, BUF_SIZE ))!=0)
		        fwrite((void*)message, 1, read_cnt, fp); */

            fclose(fp);
        }
        else{
            write(sock, message, strlen(message));
            str_len=read(sock, message, BUF_SIZE-1);
            message[str_len]=0;
            printf("Message from server\n%s\n", message);
        }
	}
	
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}