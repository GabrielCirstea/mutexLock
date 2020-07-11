#ifndef _MY_SEM
#define _MY_SEM

#include <stdlib.h>
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

// semaforul are o lista de asteptare pt thread-uri
// private:
void sem_enqueue(Sem *sem);	// adaugare in lista de asteptare
void sem_dequeue(Sem *sem);	// eliminare din lista/coada
int thread_wait(char *cond,Sem *sem);	// pune thread-ul sa astepte
int thread_signal(Sem *sem);			// scoate thread-ul din astpetare
// public:
int sem_init(Sem *sem,int pshared, int N);	// initiaza semaforul
void sem_wait(Sem *sem);					// asteapta la semafor
void sem_post(Sem *sem);					// pleaca de la semafor
int sem_destroy(Sem *sem);					// se elimina semaforul

#endif // #if
