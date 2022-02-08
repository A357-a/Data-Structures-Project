#include "json.h"
int glbl=1;
bool node::is_no_attr(){
    if(attr.empty()== 1)
            return 1;
        return 0;
}

int node::num_parent()
{
    int num=0;
    node *temp=new node;
    temp=previous;
    while(temp!=nullptr)
    {
        num++;
        temp=temp->previous;
    }
    return num;
}

string cout_tag(string s)
{
    int i =1;
    string tag;
    if(s[1] == '/')
    {
        i=2;
    }
    while((s[i] !='>') && (s[i] !=' ') && (i != s.length()-1))
    {
        if(s[i] == '/')
        {
            break;
        }
        tag += s[i];
        i++;
    }
    return tag;
}

int is_attribute(string s) //this function searching for attribute if found return its starting index else return 0
{
    for(int i=1; i<s.length(); i++)
    {
        if(s[i] == ' ')
            return i+1;
    }
    return 0;
}

vector<string> cout_attribute(string s, int index)
{
    int i=index;
        string option;//input ----> name1="1" name2="2">
        string value;//output ----->  {"@name1":"1", "@name2": "2"
        vector<string>v1;
         while(i<s.length())
        {
            if(s[i] == '=')
            {
                v1.push_back(option);
                option.clear();
                value.clear();
            }
            else if(s[i] == ' ' || s[i] == '>')
            {
                v1.push_back(value); //"@name1":"1",
                option.clear();
                value.clear();
            }
            else
            {
                if(i == s.length()-2 && s[i] =='/')
                {
                    //do nothing
                }

                else
                {
                    option += s[i];
                    value += s[i];
                }

            }

            i++;

        }
        return v1;
}

int is_duplicate(node *child)
{
    node *parent=new node;
    parent = child->previous;
    int num=0;

    std::list<node*>::iterator i;

    if(parent != nullptr)
    {
        for(i=parent->next.begin(); i!=parent->next.end(); i++)
        {
            if(*i == child)
            {
                i++;
                while(i!=parent->next.end())
                {

                    if(child->tag == (*i)->tag)
                    {
                        num++;
                        (*i)->duplicate =1;
                    }
                    else
                    {
                        break;
                    }

                    i++;
                }
                break;
            }

        }
    }
    return num;
}

node* create_tree(vector<string> arr, int arrSize)
{

    int i=0;
        vector<string>v;
        stack<string>open_tag;
        stack<node*>open_nodes; //here we create stack of pointers to nodes
        int index;
        node *root=new node;

        while(i<arrSize)
        {
                if(arr[i][0]=='<' && arr[i][1] != '/' && i==0 &&(arr[i][1] != '!' && arr[i][1] != '?'))//------>opening tag for first iteration only---->root node
                {
                    /*if(arr[i][1] != '!' || arr[i][1] != '?')
                    {
                        continue;
                    }*/

                    root->tag=cout_tag(arr[0]);
                    open_tag.push(cout_tag(arr[i]));
                    open_nodes.push(root);
                    index= is_attribute(arr[i]);
                    if(index != 0)
                    {
                        v=cout_attribute(arr[i],index);
                        for(int i=0; i<v.size(); i+=2)
                        {
                            attribute *anode= new attribute;
                            anode->option=v[i];
                            anode->value=v[i+1];
                            root->attr.push_back(*anode);

                        }
                    }

                }
                else if(arr[i][0]=='<' && arr[i][1] != '/' &&(arr[i][1] != '!' && arr[i][1] != '?')) //for rest opening tags in the tree (not including root node)
                {
                    if(arr[i][arr[i].length()-2] =='/')
                    {
                    node *branch= new node;
                    open_nodes.top()->next.push_back(branch);/*note: open_nodes stores pointers to nodes
                                                            and branch is a pointer to new node created
                                                              so when dealing with objects we use (.)
                                                              and when dealing with pointers we use (->)*/

                    branch->previous=(open_nodes.top());//--> here the parent knows his child and child knows his father
                    branch->tag=cout_tag(arr[i]);

                    //open_tag.push(cout_tag(arr[i]));
                    //open_nodes.push(branch);
                    index= is_attribute(arr[i]);

                    if(index != 0)
                    {
                        v=cout_attribute(arr[i],index);
                        for(int i=0; i<v.size(); i+=2)
                        {
                            attribute *anode= new attribute;
                            anode->option=v[i];
                            anode->value=v[i+1];
                            branch->attr.push_back(*anode);

                        }
                    }

                    }
                    else
                    {
                    node *branch= new node;
                    open_nodes.top()->next.push_back(branch);/*note: open_nodes stores pointers to nodes
                                                            and branch is a pointer to new node created
                                                              so when dealing with objects we use (.)
                                                              and when dealing with pointers we use (->)*/

                    branch->previous=(open_nodes.top());//--> here the parent knows his child and child knows his father
                    branch->tag=cout_tag(arr[i]);

                    open_tag.push(cout_tag(arr[i]));
                    open_nodes.push(branch);
                    index= is_attribute(arr[i]);

                    if(index != 0)
                    {
                        v=cout_attribute(arr[i],index);
                        for(int i=0; i<v.size(); i+=2)
                        {
                            attribute *anode= new attribute;
                            anode->option=v[i];
                            anode->value=v[i+1];
                            branch->attr.push_back(*anode);

                        }
                    }

                    }



                }
                else if(arr[i][0]=='<' && arr[i][1] == '/') //closing tag
                {
                    if(open_tag.top()==cout_tag(arr[i]))
                    {
                        open_tag.pop();
                        open_nodes.pop();

                    }
                }
                else if(arr[i][0] != '<')//string is a text inside the last tag opened
                {
                    if(open_nodes.top()->text=="")
                    {
                        open_nodes.top()->text=arr[i];
                    }
                    else
                    {
                        open_nodes.top()->text+=" "+ arr[i];
                    }
                    //assuming the text always comes before opening any other tag inside parent tag

                }
         i++;
        }
        return root;
}

string cout_json(node* root)
{
    string space;
        string space1;
        for(int i=0 ;i<root->num_parent();i++)
        {
            space+="    ";
        }
        space1=space+"    ";
        string s;

            if(root->duplicate == 0 )
            {
                s+="\n"+ space+ "\""+root->tag + "\":";
                if(is_duplicate(root)!=0)
                {
                    s+=" [";
                }
            }
            if(root->previous==nullptr && root->next.size()==0)//handling 1 node in tree
            {
                if(root->text=="")
                {
                        s+=" \"\"";
                }
                else
                {
                    s+=" \""+ root->text +"\"";
                }
            }

            else if(root->attr.empty() && root->next.empty())//no attribute and no childen
            {
                s += '"' + root->text + '"';
                if(root->previous->next.size()>1)
                {
                    //if(!is_duplicate(root)==0 && root->duplicate==0)
                    //{
                    if(!is_duplicate(root)==0 || !root->duplicate==0)
                    {
                        if(is_duplicate!=0)
                        {
                            s+=",";
                        }

                    }
                    else
                        s+=",";

                    //}


                }
            }


            else if(root->attr.size() != 0 || root->text != "") //with or without attribute and with or without text
            {
                s += "{\n    ";

                if(root->attr.size() != 0)
                {
                    for(auto i=root->attr.begin(); i!= root->attr.end(); i++)
                    {
                        s += space+"\"@" + i->option + "\":"+ i->value +",\n    ";
                    }

                }
                if(root->text != "")
                {
                s += space+ "\"#text\":\"" + root->text + "\"";
                if(!root->next.empty())
                {
                    s+=",";
                }
                }


            }
            else if(root->next.size() !=0)
            {
                s +="{\n    ";
            }
            /*else if(root->next.size()==0 && root->text=="" && root->attr.size()==0)
            {
                s+=" \"";
            }*/

        return s;
}

string iteration(node* root, string& out)
{
    out += cout_json(root);
        string space;
        std::list<node*>::iterator i;
        for (i = root->next.begin(); i != root->next.end(); i++)
        {

            glbl++;
            iteration(*i, out);

        }
        for(int i=0 ;i<root->num_parent()+1;i++)
        {
            space+="    ";
        }
        if (!root->attr.empty() || !root->next.empty())
        {
            out+="\n"+space+"}";
            if(root->duplicate==1 || is_duplicate(root)!=0)
            {
                out+=",";
            }

        }
            if(root->duplicate==1 && is_duplicate(root)==0)
            {
                out+="\n"+space+"]";
            }

        return out;
}
