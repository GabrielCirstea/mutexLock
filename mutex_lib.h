#ifndef _MY_MUTEX
#define _MY_MUTEX

//implementarea functiilor de mutex
//doar mutex
#include <atomic_ops.h>

#define test_and_set(x) AO_test_and_set(x)

//typedef enum Bool {false, true} bool;
typedef char mutex_meu;

int mutex_init_meu(mutex_meu *mtx);

void mutex_lock_meu(mutex_meu *mtx);

void mutex_unlock_meu(mutex_meu *mtx);

#endif
