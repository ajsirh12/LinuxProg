#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

struct Data{
    int x;
    int y;
};

void *t_function(void *data){
    struct Data data1;

    data1 = *((struct Data*)data);

    return (void*)(data1.x + data1.y);

}

int main(){
    pthread_t thread;
    int ret;
    int status;
    struct Data *data;
    
    data = (struct Data*)malloc(sizeof(struct Data));
    data->x = 11;
    data->y = 8;

    ret = pthread_create(&thread, NULL, t_function, (void*)&data);

    if(ret < 0){
        perror("pthread_create()\n");
        exit(-1);
    }

    pthread_join(thread, (void**)&status);
    printf("return value: %d\n", status);

    return 0;
}