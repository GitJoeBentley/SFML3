#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;


int interpretWord(map<char, int>& letterCount, string word);
void getLetterCount(ifstream& fin, map<char, int>&);
bool cmp(pair<char, int>& a, pair<char, int>& b);
void sort(map<char, int>& M);


int main()
{
    const string filename = "../resources/wordfile.bin";
    int sum;
    //int letterCount[26] = {0};
    map<char,int> letterCount;
    for (char ch = 'a'; ch <= 'z'; ch++) letterCount[ch] = 0;
    vector<string> words;
    multimap<int,string> distribution;
    char word[6];

    ifstream fin(filename,ios_base::binary);
    if (!fin)
    {
        cerr << "Cannot open " << filename << endl;
        exit(1);
    }



    getLetterCount(fin,letterCount);
    //for (auto i = 0; i < 26; i++) cout << char('a' + i) << "  " << letterCount[i] << endl;
    for (char ch = 'a'; ch <= 'z'; ch++)
        cout << ch << ' ' << letterCount[ch] << endl;

    while (fin.read(word,5))
    {
        word[5] = 0;
        sum = interpretWord(letterCount, word);
        distribution.insert(pair<int,string>(sum, word));
    }

    fin.close();
    int cnt = 0;

    for (auto rIt = distribution.crbegin(); cnt < 10; ++rIt, ++cnt)
    {
        cout << cnt << ' ' << rIt->first << "  " << rIt->second << endl;
    }
    sort(letterCount);
    cnt = 0;
    string wrd;
    for (auto rIt = distribution.crbegin(); cnt < 10; ++rIt)
    {
        wrd = rIt->second;
        if (wrd.find('t')==string::npos and wrd.find('r')==string::npos and
            wrd.find('a')==string::npos and wrd.find('i')== string::npos and
            wrd.find('n') !=string::npos)
        {
            cnt++;
            cout << cnt << ' ' << rIt->first << "  " << rIt->second << endl;
        }

    }
}

//void getLetterCount(ifstream& fin, int* letterCount)
void getLetterCount(ifstream& fin, map<char, int>& letterCount)
{
    char ch;
    while (fin.read(&ch,1))
    {
        if (ch=='a' or ch=='t' or ch=='r' or ch =='i' or ch =='n') continue;
        //letterCount[static_cast<int>(ch-'a')]++;
        letterCount[ch]++;
    }
    fin.clear();
    fin.seekg(0L);
}

//int interpretWord(int* letterCount, string word)
int interpretWord(map<char, int>& letterCount, string word)
{
    char ch;

    sort(word.begin(),word.end());
    auto it2 = unique(word.begin(),word.end());
    word.resize(distance(word.begin(),it2));
    int sum = 0;
    for (auto it = word.cbegin(); it != word.cend(); ++it)
    {
        ch = *it;
        sum += letterCount[ch];
    }
    return sum;
}

bool cmp(pair<char, int>& a,
         pair<char, int>& b)
{
    return a.second < b.second;
}

void sort(map<char, int>& M)
{
    // Declare vector of pairs
    vector<pair<char, int> > A;

    // Copy key-value pair from Map
    // to vector of pairs
    for (auto& it : M)
    {
        A.push_back(it);
    }

    // Sort using comparator function
    sort(A.begin(), A.end(), cmp);

    // Print the sorted value
    //for (auto& it : A) {
    for (auto it = A.crbegin(); it != A.crend(); ++it)
    {

        cout << it->first << ' '
             << it->second << endl;
    }
}
