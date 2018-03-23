#include <iostream>
#include <cstdlib>
#include <time.h>
using namespace std;

void printArticle();

int main()
{
    string article = "An operating system (OS) is system software that manages computer hardware and software 
    resources and provides common services for computer programs. Time-sharing operating systems schedule 
    tasks for efficient use of the system and may also include accounting software for cost allocation of 
    processor time, mass storage, printing, and other resources. For hardware functions such as input and 
    output and memory allocation, the operating system acts as an intermediary between programs and the 
    computer hardware, although the application code is usually executed directly by the hardware and 
    frequently makes system calls to an OS function or is interrupted by it. Operating systems are found on 
    many devices that contain a computer – from cellular phones and video game consoles to web servers and 
    supercomputers.";

    cout << "\nWelcome to the Parent and Child Processes w/ fork & wait Program!!"<<endl;
    cout << "\n\t\t\t\t\t\t\tArticle From Wikipedia"<<endl;
    printArticle();   




    return 0;
}

//--------------------------------------------------//
// Function to Print the Article
void printArticle()
{
    cout << "\tAn operating system (OS) is system software that manages computer hardware and software resources and provides common services for computer programs.\n"
         << "\tTime-sharing operating systems schedule tasks for efficient use of the system and may also include accounting software for cost allocation of processor\n"
         << "time, mass storage, printing, and other resources.\n"
         << "\tFor hardware functions such as input and output and memory allocation, the operating system acts as an intermediary between programs and the computer\n"
         << "hardware, although the application code is usually executed directly by the hardware and frequently makes system calls to an OS function or is\n"
         << "interrupted by it. Operating systems are found on many devices that contain a computer – from cellular phones and video game consoles to web servers\n"
         << "and supercomputers.\n"
         << "\tThe dominant desktop operating system is Microsoft Windows with a market share of around 82.74\%. macOS by Apple Inc.is in second place (13.23\%),\n" 
         << "and the varieties of Linux are collectively in third place (1.57\%). In the mobile (smartphoneand tablet combined) sector, use in 2017 is up to 70\% of\n"
         << "Google's Android and according to third quarter 2016 data, Android on smartphones is dominant with 87.5 percent and a growth rate 10.3 percent per year,\n"
         << "followed by Apple's iOS with 12.1 percent and a per year decrease in market share of 5.2 percent, while other operating systems amount to just 0.3 percent.\n"
         << "Linux distributions are dominant in the server and supercomputing sectors. Other specialized classes of operating systems, such as embedded and real-time\n"
         << "systems, exist for many applications.\n"<<endl;
}