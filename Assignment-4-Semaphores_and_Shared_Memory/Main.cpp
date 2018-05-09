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
enum {mySemaphore}; // set up names of my semaphore(s)



int rand();

void calculate(SEMAPHORE &, char *, char);
void parent_cleanup(SEMAPHORE &, int);

int main(){
	
	int shmid;
	char *shmBUF;

	string decision;
	int arrayPID [4]; 
	
	// Constructing an Object with 2 semaphores (sem is set to zero when initialize)
	SEMAPHORE sem(1); 

	// Incrementing Semaphores
	sem.V(mySemaphore); 
	sem.V(mySemaphore);

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
						//cout << "(!wq to Quit) : \n";
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
	sem.P(mySemaphore);
	if(*shmBUF == '1')
	{
		*shmBUF = '2';
		value = V;
	}
	else
	{
		value = U;
	}
	cout << "Testing: "<< *shmBUF << ", " << value << endl;

	// Setting the seed for a random generator
	srand (time(NULL));
	do
	{
		randomGenerator = rand()%10000000 + 1;
		//cout << childName << " Generated: " << randomGenerator <<" Working on "<< value << endl;
	}
	while(randomGenerator >= 100 || value % randomGenerator != 0);
	cout << "Child: "<< childName << " is done!" << endl;
	
	// Freeing up V or U depending on *shmBUF
	if(value == V)
	{
		*shmBUF = '1';
	}
	
	sem.V(mySemaphore);
} 

void parent_cleanup (SEMAPHORE &sem, int shmid) 
{

	int status;			/* child status */
	wait(0);	/* wait for child to exit */
	shmctl(shmid, IPC_RMID, NULL);	/* cleaning up */
	sem.remove();
} // parent_cleanup
