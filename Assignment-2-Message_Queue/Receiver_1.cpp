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
    string messageFromQueue, identifier, realMessage;

    // Grabbing the existing queue from the other program
	int qid = msgget(ftok(".",'u'), 0);
	cout << "Queue Found, now waiting.....\n" <<endl;

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
            // Sending Quit to 997
            strcpy(msg.message, "Receiver 1 Quitted");
            msg.mtype = 1;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);

            // Sending Quit to Queue Center
            strcpy(msg.message, "Receiver 1 Exited Successfully!");
            msg.mtype = 326;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);

            cout << "\nQuiting Program....."<<endl;
            keepGoing = false;
        }
        else
        {
            if (identifier.compare("997") == 0)
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
