#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "semaphore_lib.h"

Sem semafor;
mutex_meu mtx;
int nrBariera = 0;	// numarul curent la bariera
void barier_point(Sem *smx)
{
	mutex_lock_meu(&mtx);
	nrBariera++;
	printf("Bariera = %d\n",nrBariera);
	if(nrBariera == 4)
	{
		int i;
		for(i=0 ;i<4;i++)
			sem_post(smx);
		printf("N = %d\n",smx->N);
		nrBariera -= 4;
	}
	mutex_unlock_meu(&mtx);
	sem_wait(smx);
}

void* fct(void *argv)
{
	printf("%lu am ajuns la bariera\n",pthread_self());
	barier_point(&semafor);
	printf("%lu plecat dde la bariera\n",pthread_self());

	pthread_exit(0);
}

void pthread_initiere(pthread_t *tid,int threadNumber)
{//creem threaduri
	int i;
	for(i = 0;i < threadNumber; i++)
    {
        if(pthread_create(&tid[i],NULL,fct,NULL))
		{
			printf("Problema la creeare thread");
		}
		else
			printf("%lu a inceput\n",tid[i]);
        
    }
}

int main()
{
    sem_init(&semafor,0,0);
	mutex_init_meu(&mtx);
    printf("Hello world!\n");
    int nrThread = 4;
    pthread_t *tid = malloc(sizeof(pthread_t) * nrThread);
    pthread_initiere(tid,nrThread);
    int i;
    for(i=0;i<nrThread;i++)
        pthread_join(tid[i],NULL);
	sem_destroy(&semafor);
    return 0;
}
