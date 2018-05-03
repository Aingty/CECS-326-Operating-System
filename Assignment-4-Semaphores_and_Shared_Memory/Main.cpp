#include <iostream>
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

const int U = 827395609;
const int V = 962094883;

enum {U_Taken, V_Taken}; // set up names of my 2 semaphores


int rand();

void producer_proc(SEMAPHORE &, char *);
void parent_cleanup(SEMAPHORE &, int);
void consumer_proc(SEMAPHORE &, char *);

int main(){
	// Setting the seed for a random generator
	srand (time(NULL));
	
	string decision;

	int shmid;
	char *shmBUF;

	// Constructing an Object with 2 semaphores (sem are set to zero when initialize)
	SEMAPHORE sem(2); 
	// Incrementing each Semaphore by 1
	sem.V(U_Taken); 
	sem.V(V_Taken);
	
	// Spawn 4 children then wait for prompt
	if(fork())
	{
		if(fork())
		{
			if(fork())
			{
				if(fork())
				{
					do
					{
						cout << "(!wq to Quit) : ";
						cin >> decision;
					} while(decision.compare("!wq") != 0);
				}
				else
				{
					cout << "Testing4" << endl;
				}
			}
			else
			{
				cout << "Testing3" << endl;
			}
		}
		else
		{
			cout << "Testing2" << endl;
		}
	}
	else
	{
		cout << "Testing" << endl;
	}
    

    return(0);
}


//-----------------------------------------------------//
void consumer_proc(SEMAPHORE &sem, char *shmBUF) {
	
	
	
	int randomGenerator;

	do
	{
		randomGenerator = rand();
		cout << "Generated: " << randomGenerator << endl;

	} while(randomGenerator <= 100); //|| randomGenerator%value==0);


	// char tmp;
	// for(int k=0; k<MAXCHAR; k++){
	// 	sem.P(TAKE_ITEM);
	// 	tmp = *(shmBUF+k%BUFFSIZE); // shmBUF = shared memeory & k%BUFFSIZE = accese which memory (K = 0 --> 9) 
	// 	sem.V(PUT_ITEM);			// However it is % of BUFFSIZE so K = 0,1,2,0,1,2,0,1,2.....
	// 	cout << "(" << getpid() << ")  " 
	// 			<< "buf[" << k%BUFFSIZE << "] "
	// 			<< tmp << endl; // <--- this endl is important becasue it will flush buffer
	// }
} // child_proc

void producer_proc(SEMAPHORE &sem, char *shmBUF) {

	// char data[128];
	// cout << "(" << getpid() << ")  Please enter a string --> ";
	// cin.getline(data, 127);

	// char input;
	// for(int k=0; k<MAXCHAR; k++){
	// 	input = data[k];
	// 	sem.P(PUT_ITEM);
	// 	*(shmBUF+(k%BUFFSIZE)) = input;
	// 	sem.V(TAKE_ITEM);
	// }
} // parent_proc

void parent_cleanup (SEMAPHORE &sem, int shmid) {

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup
