# Obiecte sincronizare
Implementarea unor obiecte de sincronizare de tip mutex si semafor.  
Pentru apelarea functiilor atomice este folosita libraria [atomic_ops](https://centos.pkgs.org/6/centos-x86_64/libatomic_ops-devel-1.2-8.gc.1.el6.x86_64.rpm.html)
Implementarea este valabila doar pentru thread-uri.

## Implementare mutex
Mutex-ul a fost implementat prin folosirea functiei atomice **test_and_set**
### API
* int mutex_init_meu(mutex_meu *mtx)
	* initializeaza un obiect de tip mutex_meu primit ca parametru
* mutex_lock_meu
	* incearca blocarea mutex-ului
	* thread-ul se opreste la pana ce reuseste sa intre in mutex
* mutex_unlock_meu
	* elibereaza mutex-ul

## Implementare semafor
Semaforul permite unui anumit numar, n, de procese sa intri in zona critica.  
Intrarea este sincronizata prin intermediul unui mutex.  
Thread-urile ce nu reusesc sa intre in zona critica vor astepta la semafor intr-o coada, pentru a pastra ordinea si a garanta ca fiecare thread se va executa.  

## API semafor
* sem_init(Sem *semafor, int pshared, int N)
	* Sem, este structura semaforului
	* pshared - inca nefolosit, se trimite valoarea 0
	* N - numarul ce thread-uri ce pot trece de semafor
* sem_wait(Sem *semafor)
	* thread-ul incearca sa treaca de semafor, daca nu mai are loc va fi pus in asteptare
* sem_post(Sem *semafor)
	* threadul iese din zona critica
	* trezeste primul thread din coada, daca este vre unul
* sem_destroy(Sem *semafor)
	* sterge obiectul semafor
