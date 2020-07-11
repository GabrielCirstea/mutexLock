#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "mutex_lib.h"
enum Bool {false, true};

//pt coada de procese
struct process{
	pthread_t pid;
	/* whatever stuff */
	char wait;
	struct process *next;
};
typedef struct Sem {
	mutex_meu mtx;	//for the mutex
	int N;			// # of slots
	struct process *listStart, *listEnd;
} Sem;
void sem_enqueue(Sem *sem)
{
	//bagam doar conditia;
	struct process *new = malloc(sizeof(struct process));
	new->pid = pthread_self();
	new->wait = 1;  //dupa deblocare, thread-ul isi sterge singur conditia
	new->next = NULL;
	if(sem->listStart == NULL)
	{
		sem->listStart = new;
		sem->listEnd = new;
	}
	else
	{
		sem->listEnd->next = new;
		sem->listEnd = new;
	}
}
void sem_dequeue(Sem *sem)
{
	if(sem->listStart == NULL);
		//return NULL;
	else
	{
		struct process *p = sem->listStart;
		if(sem->listStart == sem->listEnd)
			sem->listEnd = NULL;
		sem->listStart = sem->listStart->next;
		
		free(p);
	}
}

int thread_wait(char *cond,Sem *sem)
{
	mutex_unlock_meu(&sem->mtx);
	while(*cond);       //aici arsteapta
    //dupa ce a asteptat se sterge de pe lista
    mutex_lock_meu(&sem->mtx);
    sem_dequeue(sem);
    mutex_unlock_meu(&sem->mtx);
	return 0;
}
int thread_signal(Sem *sem)
{//anunta threadul ca poate trece la treaba
	sem->listStart->wait = 0;
}
Sem *sem;
int sem_init(Sem *sem,int pshared, int N)
{
	//sem = malloc(sizeof(Sem));
	sem->N = N;
	sem->mtx = false;
	sem->listStart = NULL;
	sem->listEnd = NULL;
	return 0;
}

#if 0
void sem_wait(Sem *sem)
{
	while(1)
	{//semaforul este verificat de fiecare proces, nu are ordine
		mutex_lock_meu(&sem->mtx);
		if(sem->N > 0)
			{
				sem->N--;
				mutex_unlock_meu(&sem->mtx);
				break;	//return
			}
		mutex_unlock_meu(&sem->mtx); 	//probabil ca poate sa lipseasca
	}
}

void sem_post(Sem *sem)
{
	mutex_lock_meu(&sem->mtx);
	sem->N++;
	mutex_unlock_meu(&sem->mtx);
}
#else
void sem_wait(Sem *sem)
{//asteapta in ordine
	mutex_lock_meu(&sem->mtx);
	sem->N--;
	if(sem->N < 0)
	{
		sem_enqueue(sem);
		thread_wait(&sem->listStart->wait,sem);		//pue thread-ul in asteptare si da realese la lock
	}
	mutex_unlock_meu(&sem->mtx);
}

void sem_post(Sem *sem)
{//se elibereaza si cheama urmatorul la rand
	mutex_lock_meu(&sem->mtx);
	sem->N++;
	if(sem->N < 0)
	{
		thread_signal(sem);
	}
	mutex_unlock_meu(&sem->mtx);
}
#endif
int sem_destroy(Sem *sem)
{
	//stergem lista
	struct process *p = sem->listStart,*t;
	while(p)
	{
		t = p->next;
		free(p);
		p = t;
	}
	//free(sem);
	//probabil vom aloca campurile structurii, dimamic
	return 0;
}



//test
#define barier_max 4
Sem semafor;
mutex_meu mtx;
int bariera = 0;
int numar = 0;
int change = 1;

void barier_point()
{
	bariera++;
	if(bariera == barier_max)
	{
		int i;
		for(i=0;i<bariera;i++)
			sem_post(&semafor); //ridicam bariera.
		bariera -= barier_max;
		printf("s-a lasat bariera\n");
		change*=-1;
		//sleep(3);
	}
}
void* fct(void *arg)
{
	mutex_lock_meu(&mtx);
		barier_point();
	mutex_unlock_meu(&mtx);
	//printf("Astept la bariera\n");
	sem_wait(&semafor); //trecem si se lasa bariera daca am fost ultimii
	printf("%lu\n",pthread_self());
	printf("nr =%d \n",numar);
	numar+=change;
	pthread_exit(0);
}

void pthread_initiere(pthread_t *tid,int threadNumber)
{//creem threaduri
	int i;
	for(i = 0;i < threadNumber; i++)
		if(pthread_create(&tid[i],NULL,fct,NULL))
		{
			printf("Problema la creeare thread");
		}
		else
			printf("%lu a inceput\n",tid[i]);
}

typedef struct{
	pthread_t *tid;
	int *threadNumber;
	char *on;
}Structura;

void* the_nice_guy(void *arg)
{//asteptam threaduri
	Structura *darve = arg;
	int i = 0;
	//while(*darve->on)
	{//asteapta threadurile pana il oprim
	//numarul de threaduri poate fi modificat
		while(i < *darve->threadNumber)
		{
			pthread_join(darve->tid[i],NULL);
			printf("%lu a terminat\n",darve->tid[i]);
			i++;
		}
	}
}

void delay()
{
	int i;
	for(i = 0 ;i<100000;i++);
}

int main()
{
	int threadNumber = barier_max * 4;
	printf("Hello world\n");
	pthread_t *tid = malloc(threadNumber * sizeof(pthread_t));
	sem_init(&semafor,0, 0);
	pthread_initiere(tid,threadNumber);				//creem threadurile
	Structura *darve = malloc(sizeof(Structura));
	darve->tid = tid;
	darve->threadNumber = &threadNumber;
	darve->on = malloc(sizeof(char));
	*darve->on = 1;
	//tipul de traba
	pthread_t cute;
	pthread_create(&cute,NULL,the_nice_guy,darve);
	//delay();
	*darve->on = 0;
	pthread_join(cute,NULL);
	
	sem_destroy(&semafor);
	printf("Numar = %d\n",numar);
	return 0;
}
