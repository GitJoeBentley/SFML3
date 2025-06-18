#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <set>
#include <vector>
using namespace std;

int main()
{
    string outfile;
    string filename = "../resources/wordfile.bin";
    char word[6];
    char ch;
    unsigned numberOfWordsToDelete = 0;
    set<string> words;
    vector<string> wordsToDelete;
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

    for (auto it = words.cbegin(); it != words.cend(); ++it)
    {

        if ((*it)[4]=='s')
        {
            cout << "Do you want to delete: " << *it << endl;
            cin >> ch;
            if (ch == 'y')
            {
                numberOfWordsToDelete++;
                wordsToDelete.push_back(*it);
            }
            else if (ch == 'q') break;
        }
    }

    for (auto it = wordsToDelete.cbegin(); it != wordsToDelete.cend(); ++it)
    {
        cout << "Deleting " << (*it) << endl;
        words.erase(*it);
    }

    cout << "Deleted " << numberOfWordsToDelete << " words" << endl;

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

