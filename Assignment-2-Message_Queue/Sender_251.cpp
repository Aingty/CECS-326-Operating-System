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
#include "get_info.h"

using namespace std;

bool validateInput(string);
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
	get_info(qid, (struct msgbuf *)&msg, size, 117);


	cout << "Welcome Sender 251"<<endl;
	while(keepGoing)
	{
		cout << "What number would you like to send to Reciever 1?";
		cout << "\n\t(\"quit\" to quit)\n\t(\"rand\" to randomly send 5 numbers divisible by 251)\nYour Number: ";
		cin >> decision;
		if(decision.compare("quit") == 0)
		{
			cout << "\nSending quit to queue...\n\tQuiting, GoodBye! :)\n";
			decision = "251: " + decision;
			strcpy(msg.message, decision.c_str());
			msg.mtype = 117;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			keepGoing = false;
		}
		else if(decision.compare("rand") == 0)
		{
			cout << "Randomizing Integer and sending 5 valid markers...." <<endl;
			int sendCount = 0;
			while (sendCount < 5)
			{
				int tempNumber = rand();
				if (tempNumber % 251 == 0)
				{
					sendCount = sendCount + 1;
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
			cout << "All 5 numbers sended......\n"<<endl;
		}
		else if(validateInput(decision))
		{
			if (stoi(decision) % 251 == 0)
			{
				cout << "Sending your number: "<<decision<<"\n\n";
				decision = "251: " + decision;
				strcpy(msg.message, decision.c_str());
				msg.mtype = 117;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
			else
			{
				cout << "The number: "<<decision<<" is NOT divisible by 251.\n"<<endl;
			}
		}
		else
		{
			cout << "Invalid Input!! please send number only!!\n\n";
		}
	} 
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