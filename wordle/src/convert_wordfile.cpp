#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int main()
{
    string infile, outfile;
    char word[6];
    cout << "Enter input filename (with path) => ";
    cin >> infile;
    ifstream fin(infile);
    if (!fin)
    {
        cerr << "Cannot open " << infile << endl;
        exit(1);
    }
    if (infile.find("wordlist.txt") == string::npos)
     {
        cerr << "Invalid input file name " << infile << endl;
        exit(2);
    }

    cout << "Enter output filename (with path) => ";
    cin >> outfile;
    if (infile == outfile)
    {
        cerr << "Input file and output file cannot have the same names" << endl;
        exit(3);
    }

    ofstream fout(outfile,ios::binary);

    while (fin >> word)
    {
        fout.write(word,5);
    }
    fin.clear();
    fin.close();
    fout.close();
    cout << "Done" << endl;
}

