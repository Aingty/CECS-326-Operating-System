#include <iostream>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

string replaceAll(string, string, string);

int main()
{
    // Declaring the article file
    ifstream inFile;
    inFile.open("article.txt");

    long childPID;
    
    // Variables to use in program
    string fullArticle, lineOfArticle, target, replace, response;
    bool keepGoing = true;
    
    // If text file is not there
    if (!inFile) 
    {
        cout << "Unable to open file!!!";
        exit(1);
    }

    // Writting text file content into a string variable
    while (inFile >> lineOfArticle) 
    {
        fullArticle = fullArticle + " " + lineOfArticle;
    }

    cout << "\nWelcome to the Parent and Child Processes w/ fork & wait Program!!"<<endl;
    while(keepGoing)
    {

        cout << "\n\n\t\t\t\t\t\t\t\t\t\tArticle From Wikipedia:"<<endl;
        cout << fullArticle <<"\n\n"<<endl;

        cout << "Target String: ";
        getline(cin, target);

        // When user wants to quit
        if (target.compare("!wq")==0)
        {
            cout << "Thank You! GoodBye!! :)" <<endl;
            exit(1);
        }
        // When user's target is not in string
        if (fullArticle.find(target) == string::npos)
        {
            cout << "\nUnable to find your target string!!! Please try again!"<<endl;
            continue;
        }

        cout << "Replacement String: ";
        getline(cin, replace);
        fullArticle = replaceAll(fullArticle, target, replace);
    }
    inFile.close();
    return 0;
}

//--------------------------------------------------//
// Find and Replace string function for Program
string replaceAll(string article, string target, string replacement)
{
    string newArticle, puncuation, temp;
    bool ispuncuated = false;
    istringstream iss(article);
    while(iss >> temp)
    {
        if (!isalpha(temp[temp.size()-1]))
        {
            puncuation = temp[temp.size()-1];
            temp = temp.substr(0,temp.size()-1);
            ispuncuated = true;
        }
        if (temp.compare(target)==0)
        {
            temp = replacement;
        }
        if (ispuncuated)
        {
            temp = temp + puncuation;
            ispuncuated = false;
        }
        newArticle = newArticle + " " + temp;
    }
    return newArticle;
}