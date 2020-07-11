# facem make file pt proiect

CC = gcc
NECESARE = semafor.o mutex.o rwlock.o
FLAGS = -g -O0 # pentru debugger

start: $(NECESARE)	# compileaza doar mutex si semafor

testmutex: testmutex.c mutex.o
	$(CC) testmutex.c mutex.o -o testmutex -l pthread

mutex_test: mutex_test.c mutex.o
	$(CC) mutex_test.c mutex.o -o mutex_test -l pthread

testsem1: testsem1.c $(NECESARE)
	$(CC) $(FLAGS) testsem1.c $(NECESARE) -o testsem1 -l pthread

testsem2: testsem2.c $(NECESARE)
	$(CC) $(FLAGS) testsem2.c $(NECESARE) -o testsem2 -l pthread

testsem3: testsem3.c $(NECESARE)
	$(CC) $(FLAGS) testsem3.c $(NECESARE) -o testsem3 -l pthread

testrw: testrw.c $(NECESARE)
	$(CC) $(FLAGS) testrw.c $(NECESARE) -o testrw -l pthread

# NECESARE
semafor.o: semaphore_lib.c semaphore_lib.h
	$(CC) $(FLAGS) semaphore_lib.c -c -o semafor.o

mutex.o: mutex_lib.c mutex_lib.h
	$(CC) $(FLAGS) mutex_lib.c -c -o mutex.o

rwlock.o: rwlock_lib.c rwlock_lib.h
	$(CC) $(FLAGS) rwlock_lib.c -c -o rwlock.o

clean:
	rm -f semafor.o mutex.o rwlock.o testmutex mutex_test testsem1 testsem2 testsem3 testrw
