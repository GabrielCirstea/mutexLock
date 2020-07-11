#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


#include "semaphore_lib.h"

Sem semafor;

void* fct(void *argv)
{
    unsigned int i;
	sleep(1);
	for(i = 0;i<100000;i++);
	printf("%u am ajuns, acum plec\n",pthread_self());
	sem_post(&semafor);
	return 0;
}

void pthread_initiere(pthread_t *tid,int threadNumber)
{//creem threaduri
	int i;
	for(i = 0;i < threadNumber; i++)
    {
        sem_wait(&semafor);
        if(pthread_create(&tid[i],NULL,fct,NULL))
		{
			printf("Problema la creeare thread");
		}
		else
			printf("%u a inceput\n",tid[i]);
        
    }
}

	int main()
{
    sem_init(&semafor,0,4);
    printf("Hello world!\n");
    int nrThread = 10;
    pthread_t *tid = malloc(sizeof(pthread_t) * nrThread);
    pthread_initiere(tid,nrThread);
    int i;
    for(i=0;i<nrThread;i++)
        pthread_join(tid[i],NULL);
	sem_destroy(&semafor);
    return 0;
}
