#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

bool validateInput(string);
int rand();

int main()
{
	// Setting the seed for a random generator
    srand (time(NULL));

	// Booleans for Menu
	bool keepGoing = true;

	// Variables to use
	string messageContent;

    // Grabbing the existing queue from the other program
    int qid = msgget(ftok(".",'u'), 0);
	
    // declare my message buffer and its size
	struct buf 
	{
		long mtype; // required
		char message[50]; // mesg content
	};
	buf msg;
	int size = sizeof(msg)-sizeof(long);

	cout << "Welcome Sender 997"<<endl;

	int number = 0;
	int randomNum = 0;
	bool receiver1_alive = true;
	bool receiver2_alive = true;

	do{
		
		do{
			randomNum = rand() % INT_MAX;
			number = randomNum;
		}while (randomNum % 997 != 0);

		messageContent = "997: " + to_string(number);
		
		// Sending to Receivers
		if (receiver1_alive)
		{
			cout << "Sending to Receiver 1: " << messageContent;
			strcpy(msg.message, messageContent.c_str());
			msg.mtype = 117;
			cout << msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << endl;
		}
		if (receiver2_alive)
		{
			cout << "Sending to Receiver 2: " << messageContent;
			strcpy(msg.message, messageContent.c_str());
			msg.mtype = 118;
			cout << msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout << endl;
		}
		
		if (receiver1_alive)
		{
			cout << "\nWaiting For Receiver 1 to receive . . . " << endl;
			msgrcv(qid, (struct msgbuf *)&msg, size, 1, 0);
			messageContent = msg.message;
			if (messageContent.compare("Receiver 1 Quitted") == 0)
			{
				receiver1_alive = false;
				cout << msg.message << endl <<endl;
			}
			else
			{
				cout << "Message Recieved." << endl << endl;
			}
		}
		if (receiver2_alive)
		{
			cout << "\nWaiting For Receiver 2 to receive . . . " << endl;
			msgrcv(qid, (struct msgbuf *)&msg, size, 2, 0);
			messageContent = msg.message;
			if (messageContent.compare("Receiver 2 Quitted") == 0)
			{
				receiver2_alive = false;
				cout << msg.message << endl <<endl;
			}
			else
			{
				cout << "Message Recieved." << endl << endl;
			}
		}

	} while(number > 100);
    return 0;
}

// -------------- Custom Functions ---------//
// Decision validation function
bool validateInput(string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        char c = input[i];
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}
