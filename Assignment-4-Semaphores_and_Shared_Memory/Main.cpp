#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

using namespace std;
const int U = 827395609;
const int V = 962094883;
const int BUFFSIZE = 3;
enum {mySemaphoreA, mySemaphoreB}; // set up names of my semaphore(s)



int rand();

void calculate(SEMAPHORE &, char *, char);
void parent_cleanup(SEMAPHORE &, int);

int main(){
	
	int shmid;
	char *shmBUF;

	string decision;
	int arrayPID [4]; 
	
	// Constructing an Object with 2 semaphores (sem is set to zero when initialize)
	SEMAPHORE sem(2); 

	// Incrementing Semaphores
	sem.V(mySemaphoreA); 
	sem.V(mySemaphoreB);
	sem.V(mySemaphoreB);

	// Allocate Memory
	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS);
	shmBUF = (char *)shmat(shmid, 0, SHM_RND);

	*shmBUF = '1';

	// Spawn 4 children then parent waits for prompt
	if((arrayPID[0] = fork()))
	{
		if((arrayPID[1] = fork()))
		{
			if((arrayPID[2] = fork()))
			{
				if((arrayPID[3] = fork()))
				{
					do
					{
						cout << "(!wq to Quit) : \n";
						cin >> decision;
					} while(decision.compare("!wq") != 0);
					for(int i = 0; i <= 3; i++)
					{
						// Killing all Child Processes
						kill(arrayPID[i], SIGKILL);
					}
					parent_cleanup(sem, shmid);
				}
				else
				{
					calculate(sem, shmBUF, 'D');			
				}
			}
			else
			{
				calculate(sem, shmBUF, 'C');
			}
		}
		else
		{
			calculate(sem, shmBUF, 'B');
		}
	}
	else
	{
		calculate(sem, shmBUF, 'A');
	}
    return(0);
}


//-----------------------------------------------------//
void calculate(SEMAPHORE &sem, char *shmBUF, char childName) 
{

	char temp;
	int value;
	int randomGenerator;
	sem.P(mySemaphoreA);
	while(*shmBUF != 4)
	{
		if(*shmBUF == '1' || *shmBUF == '2')
		{
			if(*shmBUF == '1')
			{
				*shmBUF = '3';
			}
			else
			{
				*shmBUF = '4';
			}
			value = V;
		}
		else if(*shmBUF == '3')
		{
			*shmBUF = '4';
			value = U;
		}
	}
	sem.V(mySemaphoreA);
	sem.P(mySemaphoreB);
	// Setting the seed for a random generator
	srand (time(NULL));
	do
	{
		randomGenerator = rand()%100000 + 1;
		cout << childName << " Generated: " << randomGenerator <<" Working on "<< value << endl;
	}
	while(randomGenerator >= 100 || value % randomGenerator != 0);
	cout << "Child: "<< childName << " is done!" << endl;
	
	// Freeing up V or U depending on *shmBUF
	if(*shmBUF == '3')
	{
		*shmBUF = '1';
	}
	else
	{
		if(value == V)
		{
			*shmBUF = '2';
		}
		else
		{
			*shmBUF = '3';
		}
	}
	sem.V(mySemaphoreB);
} 

void parent_cleanup (SEMAPHORE &sem, int shmid) 
{

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup
