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
    // Booleans for Menu
	bool keepGoing = true;

	// Variables to use
    string messageFromQueue;
    string identifier;
	string realMessage;

    // Using ftok() to generate a queue
	int qid = msgget(ftok(".",'u'), IPC_EXCL|IPC_CREAT|0600);
    cout << "Queue Created, now waiting.....\n" <<endl;

    // declare my message buffer and its size
	struct buf 
	{
		long mtype; // required
		char message[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

    while(keepGoing)
    {
        msgrcv(qid, (struct msgbuf *)&msg, size, 117, 0);
        messageFromQueue = msg.message;
        identifier = messageFromQueue.substr(0,3);
        realMessage = messageFromQueue.substr(5);

        if(realMessage.compare("quit") == 0)
        {
            keepGoing = false;
            cout << "\nQuiting Program....."<<endl;

            // now safe to delete message queue
	        msgctl (qid, IPC_RMID, NULL);
        }
        else
        {
            if (identifier.compare("997"))
            {
                cout << identifier <<"'s Message Received: "<< realMessage <<endl;
                strcpy(msg.message, "Roger Roger from Receiver 1");
                msg.mtype = 1;
			    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
            }
            else
            {
                cout << identifier <<"'s Message Received: "<< realMessage <<endl;
            }
        }
    }
    return 0;
}
