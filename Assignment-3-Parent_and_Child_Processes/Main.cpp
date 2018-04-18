#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

void replaceAll(string, string, string);

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

        cout << "Replacement String: ";
        getline(cin, replace);

        childPID = fork();
        if (childPID == 0)
        {
            replaceAll(fullArticle, target, replace);
            exit(1);
        }  
        wait(0); 
    }
    inFile.close();
    return 0;
}

//--------------------------------------------------//
// Find and Replace string function for Program
void replaceAll(string article, string target, string replacement)
{
    int replacementCount = 0;
    string newArticle, puncuation, temp, temp2;
    bool ispuncuated = false;
    // While loop of replacementCount == 0 for injected bug
    //      Infinite loop when there is no word to replace
    while(replacementCount==0)
    {
        newArticle = "";
        //Use to iterate through the string to get one word at a time
        istringstream iss(article);
        while(iss >> temp)
        {
            // If statement to check if the word is at end of sentence IE: this word at END.
            //    Then remove the puncuation so it compares "END" only but will add puncuation later.
            if (!isalpha(temp[temp.size()-1]))
            {
                puncuation = temp[temp.size()-1];
                temp = temp.substr(0,temp.size()-1);
                ispuncuated = true;
            }
            temp2 = temp; // This is to prevent replacing all words to lowercase, 
                         //  so have this to transform all to lowercase
                         
            // Transform the temporary string/word into all lowercase for comparison
            transform(temp2.begin(), temp2.end(), temp2.begin(), ::tolower);

            if (temp2.compare(target)==0)
            {
                temp = replacement;
                replacementCount++;
            }
            if (ispuncuated)
            {
                temp = temp + puncuation;
                ispuncuated = false;
            }
            newArticle = newArticle + " " + temp;
        }
        if(replacementCount == 0)
        {
            cout << "..." << endl;
        }
    }
    cout << "\n\n\t\t\t\t\t\t\t\t\t\tNew Article From Wikipedia:"<<endl;
    cout << newArticle <<endl;
    cout << "\n\tAmount of Replacement(s): "<<replacementCount<<endl;
}