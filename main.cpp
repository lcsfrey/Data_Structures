#include <iostream>
#include "StringTrie.h"

#include <fstream>
#include <ctime>

using namespace std;

int main()
{
cout << "\n----------------Section 3----------------" << endl;

ifstream myFile;
myFile.open("GreatExpectations.txt");

int wordCount = 1;
int pageCount = 1;
char c;
string tempWord;
StringTrie book;

clock_t start = clock();
while(!myFile.eof())
{
    myFile >> tempWord;
    if(tempWord == "----------------------------------------")
    {
        pageCount++;
        wordCount = 1;
    }
    else
    {
        c = tempWord[0];
        if(c < 90 && c > 63)
        {
            c = c + 32;
        }
        tempWord = c + tempWord.substr(1, tempWord.size() - 1);
        book.addWord(tempWord);
        wordCount++;
    }
}
clock_t duration = clock() - start;
myFile.close();

cout << "Time taken to build index: " << duration / (double)CLOCKS_PER_SEC << " seconds." << endl;
cout << "Number of unique keys in index: " << book.getNumberUniqueWords() << endl;
cout << "Number of total keys in index: " << book.getNumberTotalWords() << endl;

book.printAll();

}
