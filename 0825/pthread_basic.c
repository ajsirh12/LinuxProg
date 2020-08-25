#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int glob_var = 6;

void *t_function(void *data){
    int id;
    int i = 0;
    pthread_t t_id;
    id = *((int*)data);

    glob_var++;

    t_id = pthread_self();

    printf("pid=%d, t_id=%ld, id=%d, glob_var=%d\n", getpid(), t_id, id, glob_var);
    sleep(2);

    return (void*)(id*id);
}

int main(){
    //posix thread id value
    pthread_t thread[2];
    int ret;
    int status;
    int a = 1;
    int b = 2;

    printf("Init glob_var = %d\n", glob_var);
    printf("pid=%d\n", getpid());

    //create posix thread
    ret = pthread_create(&thread[0], NULL, t_function, (void*)&a);

    if(ret<0){
        perror("Error: pthread_create()");
        exit(-1);
    }

    ret = pthread_create(&thread[1], NULL, t_function, (void*)&b);

    if(ret<0){
        perror("Error: pthread_create()");
        exit(-1);
    }

    pthread_join(thread[0], (void**)&status);
    printf("thread_join: %d\n", status);
    pthread_join(thread[1], (void**)&status);
    printf("thread_join: %d\n", status);

    printf("main thread: glob_var=%d\n", glob_var);

    return 0;
}
