#include "compression.h"
using namespace std;

string substring(string s,int index,int len,int stringlen){
    string ss="";
    for (int i=index;i<(index+len);i++){
        ss+=s[i];
        if (i==stringlen) break;
    }
    return ss;
}

string b2s(string z){//from binary to string
    unsigned char bb=0;
    string ss="";
    for(unsigned int i=0;i<z.length();i++){
        bb=z[i];
        for(int j=0;j<8;j++){
        if (readbit(bb,7-j)==1)ss+="1";
           else ss+="0";
      }
    }
   // cout<<ss;
    return ss;
}

treenode::treenode(char ch,int frequency){
    this->ch=ch;
    this->frequency=frequency;
    k=1;//is leaf
    left=NULL;
    right=NULL;
}

treenode::treenode(){}

treenode* treenode::addparentnode(treenode*left,treenode*right){
    treenode*p=new treenode();
    p->frequency=left->frequency+right->frequency;
    p->left=left;
    p->right=right;
    k=0;//not leaf
    p->ch='\0';
    return p;
}

bool treenode::isleaf(treenode*n){
    return n->left==NULL && n->right==NULL;
}

int treenode::getfrequency(){
    return frequency;
}

char treenode::getchar(){
    return ch;
}

treenode* treenode::getleft(){
    return left;
}

treenode* treenode::getright(){
    return right;
}

treenode* treenode::setnode(char ch,int frequency,treenode*left,treenode*right){
    treenode*p=new treenode();
    p->ch=ch;
    p->frequency=frequency;
    p->left=left;
    p->right=right;
    return p;
}

bool::compare::operator()(treenode* n,treenode* m){
    return n->getfrequency() > m->getfrequency();
}

void tree::encoding(treenode* n,string c, map<char,string>&newcode){
    if(n==NULL) return;
       //to check if it is leaf or not
       if(n->isleaf(n)) {
           if (c!="") newcode[n->ch] = c;
           else newcode[n->ch] = "1";
       }
       encoding(n->left,c+"0",newcode);
       encoding(n->right,c+"1",newcode);
}

queue<unsigned char> tree::maketree(string c, map<char,string> &codes,int &sizee){
    queue<unsigned char> kj;
    if (c=="") return kj;
    //to find the frequency of every char
    map<char,int>tb;//map that save avery char with its frequency
    int frequency=0;
    for (unsigned int i=0;i<c.length();i++){
        frequency= count(c.begin(),c.end(),c[i]);
        if ((frequency!=0) && (tb.count(c[i])==0)) tb[c[i]]=frequency;
    }
    //to push the nodes
    // i will push all of them first
    treenode n;
    priority_queue<treenode*,vector<treenode*>,compare>q;//this queue will save the nodes
    for(auto i=tb.begin();i!=tb.end();i++) {
        q.push(n.setnode(i->first,i->second,NULL,NULL));
    }

    //now to make the parents nodes
    while (q.size()!=1){
        treenode* left=q.top();
        q.pop();
        treenode* right=q.top();
        q.pop();
        q.push(n.addparentnode(left,right));
    }
    root=q.top();//the root of huffman tree so this is the tree


    map<char,string> newcode;// a map that save every char with its code
    tree n1;
    n1.encoding(root,"",newcode);

    //now the text after comp
    string s="";
    for(unsigned int i=0;i<c.length();i++){
        s+=newcode[c[i]];
    }

    //to convert to from binary string to binary bits
    //to push binary numbers in queue
    unsigned char bb=0;
    string ss;
    string sss;
    sizee = s.length()%8;
    sizee = 8-sizee;
    for(unsigned int i=0;i<s.length();i+=8){
        //ss=s.substr(i,8);
        ss=substring(s,i,8,s.length());
        for(int j=0;j<8;j++){
            sss=ss[j];
            if(sss=="1") setbit(bb,7-j);
            else clrbit(bb,7-j);
      }
       kj.push(bb);
    }
    codes = newcode;
    return kj;
}

decnode::decnode(char val,char c){
        value=val;
        ch=c;
        left=NULL;
        right=NULL;
}

decnode* decnode::addnode(decnode* n,char val){
       static bool flag=0;

        if ((n->right!=NULL)&&(val=='1')&&flag==0)  {

                return  n->right;

        }
        else if(n->right==NULL&&val=='1'&&flag==0) {
            n->right=new decnode(val,'\0');
                return n->right;

        }
        else if (n->left!=NULL&&val=='0'&&flag==0) {

               return  n->left;

        }
         else if(n->left==NULL&&val=='0'&&flag==0) {
                n->left=new decnode(val,'\0');
               return n->left;

         }
         else if(val=='&'&&flag==0){
            flag=1;
            return n;
         }

         else if(flag==1) {

            n->ch=val;
            flag=0;
            return n;
         }
}

dectree::dectree(){
        decroot=NULL;
    }

decnode* dectree::add(char value,decnode*n){
        if (decroot==NULL) {
                decroot=new decnode('\0','\0');
                return decroot;
        }
        else  return n->addnode(n,value);

    }


void writerecover(string filename,string z){
fstream newfile;
    string ssss="";
    unsigned char zx;
    newfile.open(filename,ios::out);
    if(!newfile) ;
    else {

        for(unsigned int i=0;i<z.length();i++){
             newfile<<z[i];
        }
    newfile.close();
    }
}

string readfile(string filename){
    string x="";
    char xx;
    fstream newfile;
    newfile.open(filename,ios::in);
    if(!newfile) cout<<"no such file";
    else {
        while (1){
            newfile>>xx;
            if(newfile.eof()) break;
             x+=xx;
        }

        newfile.close();
    }
    return x;
}

void dectree::makedectree(string filename,string compfile,string recover){
    decnode* p;
    p=add('\0',p);

    int sizee=0;
    char xx;
    fstream newfile;
    newfile.open(filename,ios::in);
    if(!newfile) ;
    else {

        string zz;
        newfile>>zz;
        sizee=atoi(zz.c_str());

        while (1){
                static bool flag1=0;
                l1: newfile>>xx;

            if(newfile.eof()) break;
             p= add(xx,p);
            if (xx=='&'&&flag1==0) {
                   flag1=1;
                    goto l1;
            }

            if (flag1==1) {

                p=decroot;
                flag1=0;
           }

        }
        newfile.close();
    }

    //now we have the tree
    //now we need to decomp
    string s=readfile(compfile);
    s=b2s(s);
    p=decroot;
    string z="";
    for(unsigned int i=0;i<s.length()-sizee;i++){
        if (s=="") return;
        else if(decroot==NULL) return;
        else if(s[i]=='1'&&p->right!=NULL) {
            p=p->right;
            if (p->right==NULL&&p->left==NULL)
            {
                z+=p->ch;
                p=decroot;
            }
        }
        else if(s[i]=='0'&&p->left!=NULL){
             p=p->left;
              if (p->right==NULL&&p->left==NULL)
              {
                  z+=p->ch;
                  p=decroot;
              }
        }
    }
         writerecover(recover,z);
    }

string dectree::makedectree2(string key,string s){
      decnode* p;
    p=add('\0',p);
    cout<<"root"<<" "<<p->ch<<endl;
    //cout<<'\0';
   // string x="";
    char xx;
    int sizee=0;
    string zz;

    zz=key[0];
//     key=key-zz;
    sizee=atoi(zz.c_str());
   /* cout<<sizee<<endl;
    cout<<key.length()<<endl;
    cout<<s<<endl;*/
   /* for(unsigned int i=1;i<key.length();i++){
           // int i=1;

          static bool flag1=0;
            l1: xx=key[i];
           // cout<<"the xx= "<<xx<<endl;
            p= add(xx,p);
           // cout<<"ghh"<<endl;
            if (xx=='&'&&flag1==0) {
                   flag1=1;
                   i++;
                    goto l1;
            }
           // cout<<xx<<endl;
            cout<<"vvvv"<<xx<<endl;
           // if(xx!='1'&&xx!='0'&&xx!='&') p=decroot;
        if (flag1==1) {
        p=decroot;
        flag1=0;
        }
    }*/
    int i=0;
    while (1){
            static bool flag1=0;
             i++;
            l1: xx=key[i];
            //key=key-xx;

            if (i==key.length()) break;
             p= add(xx,p);
              if (xx=='&'&&flag1==0) {
                   flag1=1;
                   i++;
                    goto l1;
            }
        cout<<"vvvv"<<xx<<endl;
           // if(xx!='1'&&xx!='0'&&xx!='&') p=decroot;
        if (flag1==1) {
        p=decroot;
        flag1=0;
        }
        }
    //now we have the tree
    //now we need to decomp
   // cout<<"the string is "<<s<<endl;
    //s=b2s(s);//the compressed file
   // s="0110001001111011100110101000111110100000";
   // cout<<"the string after read it "<<endl<<s<<endl;
    //0110001001111011100110101000111110100000
    p=decroot;
    string z="";
     for(unsigned int i=0;i<s.length()-sizee;i++){
        if (s=="") return "";
        else if(decroot==NULL) return "";
        else if(s[i]=='1'&&p->right!=NULL) {
            p=p->right;
            if (p->right==NULL&&p->left==NULL)
            {
                z+=p->ch;
                p=decroot;
            }
        }
        else if(s[i]=='0'&&p->left!=NULL){
             p=p->left;
              if (p->right==NULL&&p->left==NULL)
              {
                  z+=p->ch;
                  p=decroot;
              }
        }
    }
        cout<<z;
        return z;
    }
