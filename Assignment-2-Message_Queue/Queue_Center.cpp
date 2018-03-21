#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

int main() 
{   
    // Using ftok() to generate a queue
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    cout << "Queue Created....\n" <<endl;

    // Variable to use
    string messageFromQueue, identifier, realMessage;
   
    // buffer with the message contents
	struct buf
	{
		long mtype; // required
		char message[50]; // mesg content
	};

	buf msg;	//initializes instance of buffer
	int size = sizeof(msg)-sizeof(long);

    // Waiting on Receivers to end before clearing queue
    msgrcv(qid, (struct msgbuf *)&msg, size, 326, 0);

    cout << msg.message << endl;

    msgrcv(qid, (struct msgbuf *)&msg, size, 326, 0);
    
    cout << msg.message << "\n"<< endl;

    cout << "All Receivers Exited, Deleting Queue now...." << endl;

    // now safe to delete message queue
    msgctl (qid, IPC_RMID, NULL);


    return 0;
}
