#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "mutex_lib.h"

#define MAX_BUFF 7      //resurse initiale

int count = MAX_BUFF;
mutex_meu mtx;

int increment(int n)
{
	mutex_lock_meu(&mtx);
	count += n;
	mutex_unlock_meu(&mtx);
	return 0;
}

int decrement(int n)
{
	mutex_lock_meu(&mtx);
	if(n>count)
	{
		mutex_unlock_meu(&mtx);
		return -1;
	}
	count-=n;
	mutex_unlock_meu(&mtx);
	return 0;
}

void* apel(void *arg)
{
	int n = rand() % MAX_BUFF;
	printf("O sa imprumutam: %d resurse\n",n);
	if(decrement(n) == 0)
	{
		printf("Am reusit, resurse ramase = %d\n",count);
		sleep(1);
		increment(n);
		printf("Am dat drumul la %d resurse, resurse = %d\n",n,count);
	}
	else
	{
		printf("Resurse insificiente.\n");
	}
	pthread_exit(0);
}

int main()
{
	int Num = 5;
	mutex_init_meu(&mtx);
	pthread_t *tid = malloc(sizeof(pthread_t) * Num);
	int i;
	for(i=0;i<Num;i++)
	{
		pthread_create(&tid[i],NULL,apel,NULL);
	}
	for(i=0;i<Num;i++)
	{
		pthread_join(tid[i],NULL);
	}
	printf("Resurse finale: %d\n",count);
	return 0;
}
