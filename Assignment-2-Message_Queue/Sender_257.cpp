#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <time.h>
#include <string>

using namespace std;

bool validateInput(string);

int main()
{
	// Setting the seed for a random generator
    int rand();
    srand (time(NULL));

	// Variables to use
	string decision;

    // Grabbing the existing queue from the other program
    int qid = msgget(ftok(".",'u'), 0);

    // declare my message buffer and its size
	struct buf
	{
		long mtype; // required
		char message[50]; // mesg content
		bool receiverRunning; //flag to check receiver status
	};

	buf msg;
	int size = sizeof(msg)-sizeof(long);

	msg.receiverRunning = true;

	cout << "Welcome Sender 257"<<endl;

	while(msg.receiverRunning)
	{
		int value = rand();

		if(value % 257 == 0)
		{
			string m = "257: " + value;
			strcpy(msg.message, m.c_str());
			msg.mtype = 118;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}
	}

	exit(0);

    return 0;
}
