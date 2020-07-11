#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "mutex_lib.h"
#include "semaphore_lib.h"

void sem_sig_hand(int sig)	// prinde semnalul si trezeste thread-ul
{
	//write(0,"Sem: am primit mesajul\n",23);
	return;
}

void sem_enqueue(Sem *sem)
{
	//bagam doar conditia;
	struct process *new = malloc(sizeof(struct process));
	new->pid = pthread_self();
	new->wait = 1;	// flag de asteptare
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
	if(sem->listStart == NULL)
		return;
	else
	{
		struct process *p = sem->listStart;
		if(sem->listStart == sem->listEnd)
			sem->listEnd = NULL;
		sem->listStart = sem->listStart->next;
		
		free(p);
	}
}

int thread_wait(char *cond,Sem *sem)	// pune thread-ul in asteptare
{
	mutex_unlock_meu(&sem->mtx);
	while(*cond);
    mutex_lock_meu(&sem->mtx);
    sem_dequeue(sem);
    mutex_unlock_meu(&sem->mtx);
	return 0;
}

int thread_signal(Sem *sem)
{
	//write(0,"Sem: trimitem semnal\n",21);
	if(sem->listStart)
		pthread_kill(sem->listStart->pid,SIGALRM);
		//sem->listStart->wait = 0;
}

int sem_init(Sem *sem,int pshared, int N)
{
	sem->N = N;
	mutex_init_meu(&sem->mtx);
	sem->listStart = NULL;
	sem->listEnd = NULL;
	return 0;
}

void sem_wait(Sem *sem)
{//asteapta in ordine
	mutex_lock_meu(&sem->mtx);
	sem->N--;
	if(sem->N < 0)
	{
		signal(SIGALRM,sem_sig_hand);
		//write(0,"n < 0\n",6);
		sem_enqueue(sem);
		mutex_unlock_meu(&sem->mtx); // unlock the mutex befor sleeping
		pause();
		//thread_wait(&sem->listStart->wait,sem);		//pune thread-ul in asteptare si da realese la lock
	}
	mutex_unlock_meu(&sem->mtx);
}

void sem_post(Sem *sem)
{//se elibereaza si cheama urmatorul la rand
	mutex_lock_meu(&sem->mtx);
	sem->N++;
	if(sem->N <= 0)
	{
		//write(0,"N < 0\n",6);
		thread_signal(sem);
		sem_dequeue(sem);
	}
	mutex_unlock_meu(&sem->mtx);
}

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
