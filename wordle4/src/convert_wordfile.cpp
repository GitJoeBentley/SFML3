#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int main()
{
    string outfile = "../resources/wordfile4.bin";
    string filename = "../resources/wordfile4.txt";
    char word[5];
    char ch;
    cout << "Enter input filename (default is ../resources/wordfile4.txt) => ";
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
    ofstream fout(outfile,ios::binary);

    while (fin >> word)
    {
        for (auto i = 0; i < 4; i++)
            word[i] = tolower(word[i]);
        fout.write(word,4);
    }
    fin.clear();
    fin.close();
}

