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
	string decision;
  string messageFromQueue;
	string realMessage;
	string identifier;

	// buffer with the message contents
	struct buf
	{
		long mtype; // required
		char message[50]; // mesg content
		bool receiverRunning; // flag to check receiver
	};

	buf msg;	//initializes instance of buffer
	int size = sizeof(msg)-sizeof(long);
	int msgRcvdCount=0;
	msg.receiverRunning = true;

	// Grabbing the existing queue from the other program
	int qid = msgget(ftok(".",'u'), 0);
	cout << "Queue Found, now waiting.....\n" <<endl;
    while(keepGoing)
    {
        msgrcv(qid, (struct msgbuf *)&msg, size, 118, 0);
        messageFromQueue = msg.message;
				identifier = messageFromQueue.substr(0,3);
        realMessage = messageFromQueue.substr(5);

				//quits on the max messages recieved = 5000
				if(msgRcvdCount==5000)
				{
						// Sending quit to 257
						msg.receiverRunning = false;
						msg.mtype = 120;
						msgsnd(qid, (struct msgbuf *)&msg, size, 0);

						// Sending Quit to 997
            strcpy(msg.message, "Receiver 2 Quitted");
            msg.mtype = 2;
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);

						// Sending quit to Queue Center
						msg.mtype = 326;
						strcpy(msg.message, "Receiver 2 Exited Successfully!!");
						msgsnd(qid, (struct msgbuf *)&msg, size, 0);
            cout << "\nQuiting Program....."<<endl;
            keepGoing = false;
        }
				else if(identifier.compare("997") == 0)
				{
					msgRcvdCount++;
					cout << identifier <<"'s Message Received: "<< realMessage <<endl;
          strcpy(msg.message, "Roger Roger from Receiver 2");
          msg.mtype = 2;
			    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
				}
        else
        {
					msgRcvdCount++;
          cout << identifier << "'s Message Received: "<<realMessage<<endl;
					msg.mtype = 120;
					msgsnd(qid, (struct msgbuf *)&msg, size, 0);
        }
    }
    return 0;
}
