#include<bits/stdc++.h>
#include<ctime>
using namespace std;

void startwordle(); // function to guess wordle from start of game
void midwordle(); // function to guess wordle from middle of game
void guess(); //function that guesses a word from list of possible words 
void input(); // function to take input and update wordle from BRAIN
void updateBRAIN(); // function to update BRAIN
void help(); // function to display help
vector<string> wordlist; // conatains all word from dictionary
vector<string> words; // represents BRAIN
string strguess; // represents the word that is guessed
set<char> charexistsgy; // represents the characters that exists for sure {(y)|(g)}

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
    while(choice < 4) {
        cout<<"\n------WELCOME TO WORDLE SOLVER---------------\n";
        cout<<"1. START from beginning\n";
        cout<<"2. START from last played state\n";
        cout<<"3. HELP \n";
        cout<<"4. EXIT\n";
        cout<<"Enter your choice: ";
        cin>>choice;
        switch(choice) {
            case 1: startwordle(); break;
            case 2: midwordle(); break;
            case 3: help(); break;
            default: cout<<"THANK YOU FOR PLAYING\n"; break;
        }
    }
    return 0;
}

int cnt = 1; // represents the number of guesses
void startwordle()
{
    charexistsgy.clear(); // clear the set of sure characters for new game
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
        cout<<"THE WORD is : "<<words[0]<<"\n";
    }
}

void midwordle()
{
    charexistsgy.clear(); // clear the set of sure characters for new game
    int cnt = 0;
    words = wordlist;
    fstream finstartvowel; 
    updateBRAIN();
    int noofwordles = 1;
    cout<<"ENTER no of wordles already guessed: "; cin>>noofwordles;
    for(int i=0;i<noofwordles;i++) {
        cnt++;
        cout<<"Enter wordle "<<cnt<<" : ";
        cin>>strguess;
        input();
    }
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
        cout<<"THE WORD is : "<<words[0]<<"\n";
    }
}

void input() // This is the brain of the code
{
    cout<<"Enter gyb for the word:\n";
    cout<<strguess<<"\n";
    string str; cin>>str;
    if(str[0]=='x'||str[0]=='X') {
        cnt--;
        return;
    }
    vector<pair<int,char>> yellow,green;
    set<char> black;
    for(int i=0;i<str.size();i++) {
        if(str[i]=='y' || str[i] == 'Y') {
            charexistsgy.insert(strguess[i]);
            yellow.push_back(make_pair(i,strguess[i]));
        }
        else if(str[i]=='g' || str[i]=='G') {
            charexistsgy.insert(strguess[i]);
            green.push_back(make_pair(i,strguess[i]));
        }
        else {
            black.insert(strguess[i]);
        }
    }
    // FILTER OUT char that definitely exist{(g)|(y)} in final word from black
    // // // exception for black {(g)|(y)}
    // //eg. 
    // //APPLE <-- PP
    // //ybggg
    // //eg.
    // //FANGA <-- A  A
    // //bybbb
    for(auto it: charexistsgy) {
        if(black.find(it)!=black.end()) {
            black.erase(it);
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
        // if black char(which definitely doesn't exist) is found in word it is discarded
        for(auto it: black) {
            for(int j = 0; j < 5; j++) {
                if(words[i][j] == it) {
                    tobeadded = false;
                    break;
                }
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

void help()
{
    cout<<"\n";
    cout<<"HELP\n";
    cout<<"How to use :\n";
    cout<<"Enter 1 to start guess of a fresh game\n";
    cout<<"Enter 2 to start guess from middle of a game\n";
    cout<<"Once inside guess mode the code will guess a word\n";
    cout<<"You need to use the word in the wordle game you're playing\n";
    cout<<"After that type the respective colour gyb for the wordle\n";
    cout<<"g = green(present at appropriate position)\n";
    cout<<"y = yellow(present but not at appropriate position)\n";
    cout<<"b = black(not prsent)\n";
    cout<<"You can enter x to skip the current word and get a new guessword\n";
    cout<<"based on the previous word and gyb sequence the code will give you new word to guess\n";
    cout<<"Once guess is over you can enter 4 to exit the game\n";
    cout<<"\n";
}