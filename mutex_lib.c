//implementarea functiilor de mutex
//doar mutex
#include "mutex_lib.h"

int mutex_init_meu(mutex_meu *mtx)
{
	*mtx = 0;
    return 0;
}

void mutex_lock_meu(mutex_meu *mtx)
{
	while(test_and_set(mtx));
}

void mutex_unlock_meu(mutex_meu *mtx)
{
	*mtx = 0;
}
