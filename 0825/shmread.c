#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>

#define SHMCOUNT 100
#define SHMSIZE sizeof(int)*SHMCOUNT
#define KEY_VALUE (key_t)1234

int main(){
    int shmid;
	void *shared_memory = (void*)0;
    int *shmaddr;
    int i;

    //step 1. shmget()
    shmid = shmget(KEY_VALUE, SHMSIZE, 0666 | IPC_CREAT);

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
    printf("Memory attached at 0x%p\n", shared_memory);
    shmaddr = (int*)shared_memory;

    //step 3. memory access
    for(i=0;i<SHMCOUNT;i++){
        printf("shmaddr: %p, data: %d\n", shmaddr+i, *(shmaddr+i));
    }

    //step 4. shmdt()
    //shmdt() 가 실패한 경우
    if(shmdt(shared_memory) == -1){
        fprintf(stderr, "shmdt() failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //step 5. shmctl(IPC_RMID)
    if(shmctl(shmid, IPC_RMID, 0) == -1){
        fprintf(stderr, "shmctl() failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}

