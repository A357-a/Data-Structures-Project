#ifndef JSON_H
#define JSON_H
#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <stack>
#include <string>

using namespace std;

class attribute{
    public:
        string option="";
        string value="";

};

class node{
public:
        string tag;
        std::list<attribute> attr;

        string text;

        bool duplicate=0;
        std::list<node*>next;

        node* previous = nullptr;
        bool is_no_attr();
        int num_parent();
};

string cout_tag(string s);
int is_attribute(string s); //this function searching for attribute if found return its starting index else return 0
vector<string> cout_attribute(string s, int index);
int is_duplicate(node *child);
node* create_tree(vector<string> arr, int arrSize);
string cout_json(node* root);
string iteration(node* root, string& out);


#endif // JSON_H
