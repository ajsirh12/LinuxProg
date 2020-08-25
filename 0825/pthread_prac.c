#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

long global_sum = 0;

typedef struct Data{
    long start;
    long end;
}Data;

void *add_function(void *data){
    Data temp = *((Data*)data);
    long result = 0;
    for(int i = temp.start;i<=temp.end;i++){
        result += i;
    }
    global_sum += result;
}

void single_calc(){
    clock_t begin, end;
    double diff;
    long result = 0;

    printf("Single\n");
    result = 0;
    begin = clock();
    for(int i=0;i<=100000000;i++){
        result += i;
    }
    end = clock();

    diff = ((double)(end - begin)/CLOCKS_PER_SEC);

    printf("data: %ld\ntime: %f\n", result, diff);
}

int main(int argc, char* argv[]){
    int maxNum = 100000000;
    int ret;
    int status;
    clock_t start, end;

    pthread_t thread[2];
    Data data[2];

    data[0].start = 1;
    data[0].end = maxNum/2;
    data[1].start = maxNum/2 + 1;
    data[1].end = maxNum;

    start = clock();
    ret = pthread_create(&thread[0], NULL, add_function, &data[0]);

    if(ret < 0){
        perror("Error: pthread_create(&thread[0])");
        exit(-1);
    }
    ret = pthread_create(&thread[1], NULL, add_function, &data[1]);

    if(ret < 0){
        perror("Error: pthread_create(&thread[1])");
        exit(-1);
    }

    pthread_join(thread[0], (void**)&status);
    pthread_join(thread[1], (void**)&status);
    end = clock();

    printf("multithread\n");
    printf("data: %ld\n", global_sum);
    printf("time: %f\n", ((double)(end - start)/CLOCKS_PER_SEC));

    single_calc();

    return 0;
}