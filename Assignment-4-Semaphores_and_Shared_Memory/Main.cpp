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
enum {mySemaphore}; // set up names of my 2 semaphores



int rand();

void calculate(SEMAPHORE &, char *);

int main(){
	// Setting the seed for a random generator
	srand (time(NULL));
	
	int shmid;
	char *shmBUF;

	string decision;
	int arrayPID [4]; 
	
	// Constructing an Object with 1 semaphore (sem is set to zero when initialize)
	SEMAPHORE sem(1); 
	// Incrementing Semaphore by 2
	sem.V(mySemaphore); 
	
	// Allocate Memory
	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(char), PERMS);
	shmBUF = (char *)shmat(shmid, 0, SHM_RND);

	*shmBUF = "1";
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
			calculate(sem, &U_Taken, &V_Taken);
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
	char temp;
	int value;
	sem.P(mySemaphore);
	temp = *shmBUF;
	if (temp.compare("1") == 0)
	{
		value = U;
		*shmBUF = "2";
	}
	else
	{
		value = V;
		*shmBUF = "1";
	}
	do
	{
		randomGenerator = rand();
		cout << "Generated: " << randomGenerator << endl;
	}
	while(randomGenerator <= 100 || randomGenerator%value == 0);
	sem.V(mySemaphore);
} 


void parent_cleanup (SEMAPHORE &sem, int shmid) 
{

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup
