#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int main()
{
    string outfile = "../resources/wordfile6.bin";
    string filename = "../resources/wordfile6.txt";
    char word[7];
    char ch;
    cout << "Enter input filename (default is ../resources/wordfile6.txt) => ";
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
        fout.write(word,6);
    }
    fin.clear();
    fin.close();
}

