#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/time.h>

#define SHMCOUNT 100
#define SHMSIZE sizeof(long)*SHMCOUNT
#define KEY_VALUE (key_t)1234

long make_shm(int shmid, long result){
	void *shared_memory = (void*)0;
    long *shmaddr;
    long data;

    //정상적으로 공유메모리가 할당되지 않은 경우
    if(shmid == -1){
        fprintf(stderr, "shmget() failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //step 2. shmat()
    shared_memory = shmat(shmid, (void*)0, 0);

    //shmat() 가 실패한 경우
    if(shared_memory == (void*)-1){
        fprintf(stderr, "shmat() failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //shmat() 가 성공한 경우, 할당된 메모리 주소글 확인
    shmaddr = (long*)shared_memory;
    *(shmaddr) += result;
    
    //step 3. memory access
    data = *(shmaddr);
    
    //step 4. shmdt()
    //shmdt() 가 실패한 경우
    if(shmdt(shared_memory) == -1){
        fprintf(stderr, "shmdt() failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    //printf("* %ld\n", *(shmaddr));
    //printf("%ld\n", result);

    return data;
}

void quit_shm(int shmid){
    //step 5. shmctl(IPC_RMID)
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        fprintf(stderr, "shmctl() failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

void single_calc(){
    struct timeval begin, end;
    double diff;
    long result = 0;

    printf("Single\n");
    result = 0;
    gettimeofday(&begin, NULL);
    for(int i=0;i<=100000000;i++){
        result += i;
    }
    gettimeofday(&end, NULL);

    diff = end.tv_sec + end.tv_usec / 1000000.0 - begin.tv_sec + begin.tv_usec / 1000000.0;

    printf("data: %ld\ntime: %f\n", result, diff);
}

int main(){
    struct timeval begin, end;
    double diff;
    long result = 0;
    long data;

    pid_t pid;
    int status;

    int shmid;
    //step 1. shmget()
    shmid = shmget(KEY_VALUE, SHMSIZE, 0666 | IPC_CREAT);

    printf("Dual\n");
    gettimeofday(&begin, NULL);
    pid = fork();

    if(pid ==-1){
        printf("Error: fork()");
        exit(-1);
    }
    else if(pid == 0){
        for(int i=0; i<=50000000;i++){
            result += i;
        }
        make_shm(shmid, result);
        exit(0);
    }
    else{
        for(int i=50000001;i<=100000000;i++){
            result += i;
        }
        wait(&status);
        data = make_shm(shmid, result);
        quit_shm(shmid);
        gettimeofday(&end, NULL);
    }

    diff = end.tv_sec + end.tv_usec / 1000000.0 - begin.tv_sec + begin.tv_usec / 1000000.0;
    printf("data: %ld\ntime: %f\n",data ,diff);

    single_calc();
    
    return 0;
}