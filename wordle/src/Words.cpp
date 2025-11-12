#include "Words.h"
#include "Constants.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cctype>
#include <ctime>
using namespace std;

using std::string;
using std::ifstream;

Words::Words()
{
    getRandomWord();
    readGuessFile();
}

void Words::readGuessFile()
{
    // Read file in Binary
    ifstream fin(GuessFile,ios::binary);
    if (!fin)
    {
        std::cerr << "Unable to open word file: " << WordFile << std::endl;
        exit(1);
    }

    char word[6];
    string sword;
    while (fin.read(word,5))
    {
        word[5] = 0;
        sword = word;
        // insert each word in uppercase
        for (auto it = sword.begin(); it != sword.end(); ++it)
            *it = toupper(*it);
        auto pr = words.insert(sword);
        if (pr.second == false) cout << *(pr.first) << endl;
    }
    fin.close();
}

void Words::getRandomWord()
{
    srand(static_cast<unsigned>(time(nullptr)));
    ifstream fin(WordFile,ios::binary);
    char word[6];
    if (!fin)
    {
        std::cerr << "Unable to open word file: " << WordFile << std::endl;
        exit(1);
    }
    fin.seekg(0, ios::end);
    auto numberOfWords = fin.tellg() / 5;
    int index = rand() % numberOfWords;
    fin.seekg(0);
    for (auto i = 0; i < index; i++)
    {
        fin.read(word,5);
    }
    fin.close();

    word[5] = 0;
    for (auto i = 0; i< 5; i++) word[i] = toupper(word[i]);

    theWord = word;
}

bool Words::isValidWord(const string& word)
{
    bool found = words.find(word) != words.end();
    return found;
}

// Evaluates guess:  assigns 5-element array, guessEvalution
// 0 = invalid Letter
// 1 = right letter, wrong place
// 2 = right letter, right place
// returns true if all 5 letters are right letter, right place
bool Words::evaluateGuess(string guess, unsigned* guessEvaluation)
{
    string theWordCopy = theWord;
    for (int i = 0; i < 5; i++) guessEvaluation[i] = 0;
    for (int i = 0; i < 5; i++)
    {
        if (guess[i] == theWordCopy[i])
        {
            guessEvaluation[i] = 2;
            guess[i] = ' ';
            theWordCopy[i] = ' ';
        }
    }
    size_t pos;
    for (int g = 0; g < 5; g++)
    {
        if (guess[g] != ' ')
        {
            pos = theWordCopy.find(guess[g]);
            if (pos != string::npos)
            {
                guessEvaluation[g] = 1;
                guess[g] = ' ';
                theWordCopy[pos] = ' ';
            }
        }
    }
    for (int i = 0; i < 5; i++)
    {
        if (guessEvaluation[i] != 2) return false;
    }
    return true;
}
