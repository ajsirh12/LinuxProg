#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 8096
#define LLIST "ls"
#define DDN "dn "

void read_childproc(int sig){
    pid_t pid;
    int status;

    pid = waitpid(-1, &status, WNOHANG);
    printf("remove proc id: %d\n", pid);
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[]){
    //시그널 핸들러 등록
    signal(SIGCHLD, read_childproc);

    FILE *read_fp;
    char buffer[BUFSIZ + 1];
    int chars_read;
    memset(buffer, '\0', sizeof(buffer));

    char* filename;
    FILE* fp;
    int read_cnt;

    char* info = "Input \"ls\" or \"dn filename\"";

    //socket file descriptor
    int serv_sock, clnt_sock;
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_addr, clnt_addr;

    pid_t pid;
    socklen_t addr_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(-1);
    }

    //step 1. socket()
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
		error_handling("socket() error");

    //step 2. bind()
    memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1 ){
		error_handling("bind() error"); 
    }

    //step 3. listen()
    if(listen(serv_sock, 10) == -1){
        error_handling("listen() error");
    }

    while(1){
        addr_sz = sizeof(clnt_addr);

        //step 4. accept()
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_sz);
        if(clnt_sock == -1){
            continue;
        }
        //클라이언트 접속 허용된 경우
        else{
            printf("new client connected...\n");
        }
        pid = fork();
        if(pid == -1){
            close(clnt_sock);
            continue;
        }
        else if(pid == 0){  //자식프로세스
            close(serv_sock);
            //데이터처리
            while((str_len = read(clnt_sock, buf, BUF_SIZE-1))!=0){
                buf[str_len] = 0;
                buf[str_len-1] = 0;
                printf("%s\n", buf);                
                if(strcmp(buf, LLIST) == 0){
                    read_fp = popen(LLIST, "r");
                    if (read_fp != NULL) {
                        chars_read = fread(buffer, sizeof(char), BUFSIZ, read_fp);
                        if (chars_read > 0) 
                        {
                            write(clnt_sock, buffer, strlen(buffer));
                        }
                        pclose(read_fp);
                    }
                }
                else if(strncmp(buf, DDN, 3) == 0){
                    filename = strtok(buf, " ");
                    filename = strtok(NULL, " ");
                    fp=fopen(filename, "rb"); 
                    while(1){
                            read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);
                            if(read_cnt<BUF_SIZE){
                                write(clnt_sock, buf, read_cnt);
                                break;
                            }
                            write(clnt_sock, buf, BUF_SIZE);
                        }
                    fclose(fp);
                }
                else{
                    write(clnt_sock, info, strlen(info));
                    //write(clnt_sock, buf, strlen(buf));
                }
            }
//            *buf = '\0';
            close(clnt_sock);
            printf("client disconnected...\n");
            return 0;
        }
        else{   //부모프로세스
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}