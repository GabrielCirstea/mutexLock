#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include "mutex_lib.h"

pthread_t tid[2];
int counter;
mutex_meu lock;


void* doSomeThing(void *arg)
{
    mutex_lock_meu(&lock);

    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d started\n", counter);

    for(i=0; i<32000;i++);

    printf("\n Job %d finished\n", counter);

    mutex_unlock_meu(&lock);

    return NULL;
}

int main(void)
{
    int i = 0;
    int err;

    if (mutex_init_meu(&lock) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    while(i < 2)
    {
        err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
        if (err != 0)
            printf("\ncan't create thread :[%s]", strerror(err));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    printf("Counter = %d\n",counter);
    return 0;
}
