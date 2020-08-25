#include<stdio.h>
#include<sys/time.h>

int main(){

    struct timeval begin, end;
    double diff;

    long result = 0;

    gettimeofday(&begin, NULL);
    for(int i=0;i<=100000000;i++){
        result += i;
    }
    gettimeofday(&end, NULL);

    diff = end.tv_sec + end.tv_usec / 1000000.0 - begin.tv_sec + begin.tv_usec / 1000000.0;

    printf("data: %ld\ntime: %f\n", result, diff);

    return 0;
}