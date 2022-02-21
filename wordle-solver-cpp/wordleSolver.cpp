#include<bits/stdc++.h>
#include<ctime>
using namespace std;

void startwordle(); // function to guess wordle from start of game
void midwordle(); // function to guess wordle from middle of game
void guess(); //function that guesses a word from list of possible words 
void input(); // function to take input and update wordle from BRAIN
void updateBRAIN(); // function to update BRAIN
vector<string> wordlist; // conatains all word from dictionary
vector<string> words; // represents BRAIN
string strguess; // represents the word that is guessed
set<int> fixedindex; // represents the index of the word that is already fixed

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
            case 2:midwordle(); break;
            default: cout<<"THANK YOU FOR PLAYING\n"; break;
        }
    }
    return 0;
}

void startwordle()
{
    fixedindex.clear(); // clear the set of fixed index for new guesses
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
        cout<<"THE WORD is : "<<words[0]<<"\n";
    }
}

void midwordle()
{
    fixedindex.clear(); // clear the set of fixed index for new guesses
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
        return;
    }
    vector<pair<int,char>> yellow,green;
    vector<char> black;
    for(int i=0;i<str.size();i++) {
        if(str[i]=='y' || str[i] == 'Y') {
            yellow.push_back(make_pair(i,strguess[i]));
        }
        else if(str[i]=='g' || str[i]=='G') {
            if(fixedindex.find(i)==fixedindex.end()) {
                green.push_back(make_pair(i,strguess[i]));
            }
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
            // // This if statement is added to make sure the one's whose index, those index are already fixed are not again checked
            // if(fixedindex.find(it.first)!=fixedindex.end()) {
            //     continue;
            // }
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
    
    // // before filtering black update fixedindex
    //after green & yellow filtering is done, we need to update fixed index to make sure the black filter doesnt confuse multiple occurence of same char as that char shouldn't be included
    //eg. 
    //APPLE
    //ybggg
    // there is b under 2nd P but P still exist, this pecularity is accordance to offical wordle game; instead of second P getting a b it could have been awarded a y but as per rules this has to be handled for b instead of y
    for(auto it: green) {
        fixedindex.insert(it.first);
    }

    // // FILTER BLACK
    for(int i = 0; i < words.size(); i++) {
        bool tobeadded = true;

        // if black char is found in word it is discarded
        // but while searching black care has to be taken to avoid the indexes that are already fixed
        // to be added should be made false if black char is found in word except the fixed indexes
        for(auto it: black) {
            for(int j = 0; j < 5; j++) {
                if(fixedindex.find(j)!=fixedindex.end()) { // if fixed index found encountered then continue or skip the loop 
                    continue;
                }
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