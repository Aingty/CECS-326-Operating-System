#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include "Get_Info.h"

using namespace std;

int rand();
void get_info(int, struct msgbuf *, int, long);

int main()
{
	// Setting the seed for a random generator
    srand (time(NULL));

	// Booleans for Menu
	bool keepGoing = true;

	// Variables to use
	string decision;

    // declare my message buffer and its size
	struct buf 
	{
		long mtype; // required
		char message[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

    // Grabbing the existing queue from the other program
    int qid = msgget(ftok(".",'u'), 0);
	
	// Patch Code to terminate Receiver 1 after kill command
	strcpy(msg.message, "251: quit");
	msg.mtype = 117;
	get_info(qid, (struct msgbuf *)&msg, size, 117);

	cout << "Welcome Sender 251"<<endl;
	while(keepGoing)
	{
		//cout << "Randomizing Integer and sending valid markers...." <<endl;
		int tempNumber = rand();
		if (tempNumber % 251 == 0)
		{
			cout.width(15);
			cout << left << tempNumber << " : Sended"<<endl;
			decision = "251: " + to_string(tempNumber);
			strcpy(msg.message, decision.c_str());
			msg.mtype = 117;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);	
		} 
		else
		{
			cout.width(15); 
			cout<< left << tempNumber << " : Trash"<<endl;
		}
	} 
    return 0;
}