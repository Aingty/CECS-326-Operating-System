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

using namespace std;

const int U = 827395609;
const int V = 962094883;
const int BUFFSIZE = 3;
enum {mySemaphoreA, mySemaphoreB}; // set up names of my 2 semaphores



int rand();

void calculate(SEMAPHORE &, char *);

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
						cout << "(!wq to Quit) : ";
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
					//calculate(sem, &U_Taken, &V_Taken);			
				}
			}
			else
			{
				//calculate(sem, &U_Taken, &V_Taken);
			}
		}
		else
		{
			calculate(sem, shmBUF);
		}
	}
	else
	{
		calculate(sem, shmBUF);
	}
    return(0);
}


//-----------------------------------------------------//
void calculate(SEMAPHORE &sem, char *shmBUF) 
{
	// Setting the seed for a random generator
	srand (time(NULL));
	char temp;
	int value;
	int randomGenerator;
	sem.P(mySemaphoreA);
	temp = *shmBUF;
	if (temp == '1')
	{
		value = U;
		*shmBUF = '2';
	}
	else
	{
		value = V;
		*shmBUF = '1';
	}
	sem.V(mySemaphoreA);
	sem.P(mySemaphoreB);
	cout << "Shared Mem: " << *shmBUF <<endl;
	do
	{
		randomGenerator = rand();
		cout << "Generated: " << randomGenerator << endl;
	}
	while(randomGenerator <= 100 || randomGenerator%value == 0);
	sem.V(mySemaphoreB);
} 

void parent_cleanup (SEMAPHORE &sem, int shmid) 
{

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup
