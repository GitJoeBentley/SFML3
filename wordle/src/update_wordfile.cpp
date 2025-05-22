#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <set>
using namespace std;

int main()
{
    string outfile;
    string filename = "../resources/wordfile.bin";
    char word[6];
    char ch;
    unsigned numberOfNewWords = 0;
    set<string> words;
    cout << "Enter input filename (default is ../resources/wordfile.bin) => ";
    ch = getchar();
    if (ch != '\n')
    {
        cin.putback(ch);
        cin >> filename;
    }
    ifstream fin(filename,ios::binary);
    if (!fin)
    {
        cerr << "Cannot open " << filename << endl;
        exit(1);
    }

    while (fin.read(word,5))
    {
        word[5]= 0;
        words.insert(word);
    }
    cout << "Read " << filename << "   Number of words = "  << words.size() << endl << endl;


    cout << "Enter words to add to the word file (\"quit\" to stop)" << endl;
    while (true)
    {
        cout << "=> ";
        cin >> word;
        for (int i = 0; i < 6; i ++) word[i] = tolower(word[i]);
        if (strlen(word) != 5)
        {
            if (word[0] == 'q' || word[0] == 'Q') break;
            else
            {
                cout << "Huh?" << endl;
                continue;
            }
        }
        if (words.find(word) == words.end())
        {
            words.insert(word);
            cout << word << " added to the word list\n";
            numberOfNewWords++;
        }
    }
    cout << endl << "Added " << numberOfNewWords << " words to the list" << endl << endl;

    fin.clear();
    fin.close();

    ofstream fout1(filename,ios::binary);
    string textfile = filename.substr(0,filename.size()-3) + "txt";
    ofstream fout2(textfile);
    for (auto it = words.cbegin(); it != words.cend(); ++it)
    {
        fout1.write(it->c_str(),5);
        fout2 << (*it) << endl;
    }
    cout << "Wrote " << words.size() << " words into " << filename << " and " << textfile << endl;
}

