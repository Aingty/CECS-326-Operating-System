#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

bool validateInput(string);

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
    while(keepGoing)
    {
        msgrcv(qid, (struct msgbuf *)&msg, size, 118, 0);
        messageFromQueue = msg.message;
				identifier = messageFromQueue.substr(0,3);
        realMessage = messageFromQueue.substr(5);
				cout << identifier <<endl;
				if(msgRcvdCount==5000)	//quits on the max messages recieved = 5000
				{
						msg.receiverRunning = false;
						msg.mtype = 118;
						msgsnd(qid, (struct msgbuf *)&msg, size, 118);
            keepGoing = false;
            cout << "\nQuiting Program....."<<endl;
        }
				else if(identifier.compare("997"))
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
          cout << "Message Received: "<<msg.message<<endl;
					msg.mtype = 118;
					msgsnd(qid, (struct msgbuf *)&msg, size, 118);
        }

    }
    return 0;
}
