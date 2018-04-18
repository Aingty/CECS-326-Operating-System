#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"

using namespace std;

const int MAXCHAR = 10;
const int BUFFSIZE = 3;
enum {PUT_ITEM, TAKE_ITEM}; // set up names of my 2 semaphores

void producer_proc(SEMAPHORE &, char *);
void parent_cleanup(SEMAPHORE &, int);
void consumer_proc(SEMAPHORE &, char *);

int main(){
	int shmid;
	char *shmBUF;

	SEMAPHORE sem(2); // Constructing an Object with 2 semaphores (sem are set to zero when initialize)
	sem.V(PUT_ITEM); // Incrementing each PUT_ITEM by 1
	sem.V(PUT_ITEM);
	sem.V(PUT_ITEM); // PUT_ITEM = 3 

	//shmget = allocate function     IPC_PRIVATE = replace ftok(".",'u') (Now programmer doesnt tell what key to use, IPC_PRIVATE will use random key)
	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS); // Setting up shared memory
	shmBUF = (char *)shmat(shmid, 0, SHM_RND); // Attach to allocated shm.

	// When forking here, child have all of parent's variable including the same pointer to same memory (shared memeory)
	if(fork()){ /* parent process */

		producer_proc(sem, shmBUF);
		parent_cleanup(sem, shmid);

	} else { // child process
		consumer_proc(sem, shmBUF);
	}

	exit(0);
} // main

void consumer_proc(SEMAPHORE &sem, char *shmBUF) {
	char tmp;

	for(int k=0; k<MAXCHAR; k++){
		sem.P(TAKE_ITEM);
		tmp = *(shmBUF+k%BUFFSIZE); // shmBUF = shared memeory & k%BUFFSIZE = accese which memory (K = 0 --> 9) 
		sem.V(PUT_ITEM);			// However it is % of BUFFSIZE so K = 0,1,2,0,1,2,0,1,2.....
		cout << "(" << getpid() << ")  " 
				<< "buf[" << k%BUFFSIZE << "] "
				<< tmp << endl; // <--- this endl is important becasue it will flush buffer
	}
} // child_proc

void producer_proc(SEMAPHORE &sem, char *shmBUF) {

	char data[128];
	cout << "(" << getpid() << ")  Please enter a string --> ";
	cin.getline(data, 127);

	char input;
	for(int k=0; k<MAXCHAR; k++){
		input = data[k];
		sem.P(PUT_ITEM);
		*(shmBUF+(k%BUFFSIZE)) = input;
		sem.V(TAKE_ITEM);
	}
} // parent_proc

void parent_cleanup (SEMAPHORE &sem, int shmid) {

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup

