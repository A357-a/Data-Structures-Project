#ifndef COMPRESSION_H
#define COMPRESSION_H
#include <iostream>
#include<stdio.h>
#include<queue>
#include<unordered_map>
#include<string>
#include<algorithm>
#include<map>
#include<vector>
#include<cstdlib>
#include<fstream>
#include <sstream>
#include "QMainWindow"
#include "QMessageBox"
#include "QFile"
#include "QTextStream"
#include "QDir"
#include "mainwindow.h"
#define setbit(reg,bit) reg|=(1<<bit)
#define clrbit(reg,bit) reg&=~(1<<bit)
#define readbit(reg,bit) ((reg>>bit)&1)

using namespace std;

string substring(string s,int index,int len,int stringlen);
string b2s(string z);
void writefile(string filename,int len, queue<unsigned char> &kj);

class treenode{
private:
    char ch;
    int frequency;
    bool k;
    treenode* left;
    treenode* right;
public:
    treenode(char ch,int frequency);
    treenode();
    treenode* addparentnode(treenode*left,treenode*right);
    bool isleaf(treenode*n);
    int getfrequency();
    char getchar();
    treenode* getleft();
    treenode* getright();
    treenode* setnode(char ch,int frequency,treenode*left,treenode*right);
    friend class tree;
};

class compare{
public:
    bool operator()(treenode* n,treenode* m);
};

class tree{
private:
    treenode*root;
public:
    void encoding(treenode* n,string c, map<char,string>&newcode);
    queue<unsigned char> maketree(string c, map<char,string> &codes,int &sizee);
};

class decnode{
public:
    char value;
    char ch;
    decnode*left;
    decnode*right;
    decnode(char val,char c);
    decnode* addnode(decnode* n,char val);
};

class dectree{
private:
    decnode*decroot;
public:
    dectree();
    decnode* add(char value,decnode*n);
    void makedectree(string filename,string compfile,string recover);
    string makedectree2(string key,string s);
};

string readfile(string filename);
#endif // COMPRESSION_H
