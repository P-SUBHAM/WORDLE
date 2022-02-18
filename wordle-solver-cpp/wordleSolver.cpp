#include<bits/stdc++.h>
#include<ctime>
using namespace std;

void startwordle(); // function to guess wordle from start 
void guess(); //function that guesses a word from list of possible words 
void input(); // function to take inpu and update wordle from BRAIN
void updateBRAIN(); // function to update BRAIN
vector<string> wordlist; // conatains all word from dictionary
vector<string> words; // represents BRAIN
string strguess; // represents the word that is guessed

int main()
{
    srand(time(0));
    fstream finwordlist; 
    finwordlist.open("assets/wordlist.txt",ios::in);
    while(finwordlist) {
        string s;
        finwordlist>>s;
        if(s.size()==5) {
            wordlist.push_back(s);
        }
    }
    int choice = 1;
    while(choice == 1 || choice == 2) {
        cout<<"------WELCOME TO WORDLE SOLVER---------------\n";
        cout<<"1. START from beginning\n";
        cout<<"2. START from last played state\n";
        cout<<"3. EXIT\n";
        cout<<"Enter your choice: ";
        cin>>choice;
        switch(choice) {
            case 1: startwordle(); break;
            case 2: cout<<"TO IMPLEMENT\n"; break;
            default: cout<<"THANK YOU FOR PLAYING\n"; break;
        }
    }
    return 0;
}

void startwordle()
{
    int cnt = 1;
    words.resize(0);
    fstream finstartvowel; 
    finstartvowel.open("assets/startvowel.txt",ios::in);
    while(finstartvowel) {
        string s;
        finstartvowel>>s;
        if(s.size()==5) {
            words.push_back(s);//These are initial words used in guessing
        }
    }
    updateBRAIN();
    guess();
    cout<<"WORD "<<cnt<<" : "<<strguess<<"\n"; 
    cnt++;
    words = wordlist;
    input();
    while(words.size()>1) {
        guess();
        cout<<"WORD "<<cnt<<" : "<<strguess<<"\n"; 
        cnt++;
        updateBRAIN();
        input();
    }
    if(words.size()==0) {
        cout<<"CANNOT SOLVE\n";
    }
    else {
        cout<<"HURRAY!!!!\n";
        cout<<"WORD "<<cnt<<" : "<<words[0]<<"\n";
    }
}

void input() // This is the brain of the code
{
    cout<<"Enter gyb for the word:\n";
    cout<<strguess<<"\n";
    string str; cin>>str;
    if(str[0]=='x'||str[0]=='X') {
        return;
    }
    vector<pair<int,char>> yellow,green;
    vector<char> black;
    for(int i=0;i<str.size();i++) {
        if(str[i]=='y' || str[i] == 'Y') {
            yellow.push_back(make_pair(i,strguess[i]));
        }
        else if(str[i]=='g' || str[i]=='G') {
            green.push_back(make_pair(i,strguess[i]));
        }
        else {
            black.push_back(strguess[i]);
        }
    }
    vector<string> newwords;
    // // FILTER GREEN
    for(int i = 0; i < words.size(); i++) {
        bool tobeadded = true;
        for(auto it: green) {
            if(words[i][it.first] != it.second) {
                tobeadded = false;
                break;
            }
        }
        if(tobeadded) {
            newwords.push_back(words[i]);
        }
    }
    words = newwords;
    newwords.resize(0);
    // // FILTER YELLOW
    for(int i = 0; i < words.size(); i++) {
        bool tobeadded = true;
        for(auto it: yellow) {
            if(words[i][it.first] == it.second) {
                tobeadded = false;
                break;
            }
            bool isycharprsent = false;
            for(int j = 0; j < 5; j++) {
                if(j==it.first) continue;
                if(words[i][j] == it.second) {
                    isycharprsent = true;
                    break;
                }
            }
            if(isycharprsent==false) {
                tobeadded = false;
                break;
            }
        }
        if(tobeadded) {
            newwords.push_back(words[i]);
        }
    }
    words = newwords;
    newwords.resize(0);
    // // FILTER BLACK
    for(int i = 0; i < words.size(); i++) {
        bool tobeadded = true;
        for(auto it: black) {
            if(words[i].find(it) != string::npos) {
                tobeadded = false;
                break;
            }
        }
        if(tobeadded) {
            newwords.push_back(words[i]);
        }
    }
    words = newwords;
    newwords.resize(0);
    updateBRAIN();
}

void guess() // Guesses a word from BRAIN
{
    int n = words.size();
    if(n==0) {
        string strguess = "NO_WORD_FOUND";
        return;
    }
    int index = 0;
    index = rand()%n;
    strguess = words[index];
}

void updateBRAIN() //UPDATES BRAIN WITH WHAT CODE THINKS
{
    
    fstream fbrain;
    fbrain.open("LiveCodeBRAIN.txt",ios::out);
    if(words.size()==0) {
        fbrain<<"EMPTY_BRAIN\n";
        return;
    }
    for(int i=0;i<words.size()-1;i++) {
        fbrain<<words[i]<<"\n";
    }
    fbrain<<words[words.size()-1];
}