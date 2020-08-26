#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define MAXNUM 1000000000
#define THREAD_CNT 4

long global_sum = 0;

typedef struct Data{
    long start;
    long end;
    pthread_mutex_t* mutx;
}Data;

void *add_function(void *data){
    Data temp = *((Data*)data);
    long result = 0;
    
    pthread_mutex_lock(temp.mutx);
    for(int i = temp.start;i<=temp.end;i++){
        global_sum += i;
    }
    pthread_mutex_unlock(temp.mutx);
}

void single_calc(){
    clock_t begin, end;
    double diff;
    long result = 0;

    printf("Single\n");
    result = 0;
    begin = clock();
    for(int i=0;i<=MAXNUM;i++){
        result += i;
    }
    end = clock();

    diff = ((double)(end - begin)/CLOCKS_PER_SEC);

    printf("data: %ld\ntime: %f\n", result, diff);
}

int main(int argc, char* argv[]){
    int ret;
    int status;
    clock_t start, end;

    pthread_mutex_t mutx;

    pthread_t thread[THREAD_CNT];
    Data data[THREAD_CNT];

    pthread_mutex_init(&mutx, NULL);

    for(int i=0;i<THREAD_CNT;i++){
        data[i].start = (MAXNUM/THREAD_CNT)*(i)+1;
        data[i].end = (MAXNUM/THREAD_CNT)*(i+1);
        data[i].mutx = &mutx;
    }

    start = clock();

    for(int i=0;i<THREAD_CNT;i++){
        ret = pthread_create(&thread[i], NULL, add_function, &data[i]);

        if(ret < 0){
            printf("Error: pthread_create(&thread[%d])\n", i);
            exit(-1);
        }
    }

    for(int i=0;i<THREAD_CNT;i++){
        pthread_join(thread[i], (void**)&status);    
    }
    
    end = clock();

    printf("multithread\n");
    printf("data: %ld\n", global_sum);
    printf("time: %f\n", ((double)(end - start)/CLOCKS_PER_SEC));

    single_calc();

    return 0;
}