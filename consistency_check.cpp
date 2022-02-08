#include "consistency_check.h"
#include "QStack"
#include "QMap"

// XML Consistency Check. Returns a boolean value whether the file is XML-consistent or not
bool check_consistency(QVector<QString> xml){
    QStack<QString> tag;
        for (int i = 0; i < xml.size(); i++) {
            QString line = xml[i];
            if (line[0] == '<') {
                if (line[1] == '/') {
                    if(!tag.empty()){
                        if (line != ("</" + tag.top())) {
                            // unmatched closing and opening tags
                            return false;
                        }
                        else {
                            tag.pop();
                        }
                    }
                    else{
                        // closing tag without opening tag
                        return false;
                    }
                }
                else if(line[1] != '!' && line[1] != '?') {
                    if(!(line[line.length()-2] == '/' && line[line.length()-1] == '>')){
                        QString temp = "";
                        for(int j=1; j<line.length(); j++){
                            if(line[j] != ' ' && line[j] != '>'){
                             temp += line[j];
                            }
                            else{
                                temp += '>';
                                break;
                            }
                        }
                        tag.push(temp);
                    }
                }
            }
        }
    return (tag.empty());
}


QMap<qint32, QString> identify_errors(QVector<QString> xml){
    QMap<qint32, QString> res;
    QStack<QString> tag;
    QStack<int> line_num;
        int i = 0;
        for (i = 0; i < xml.size(); i++) {
            QString line = xml[i];
            if (line[0] == '<') {
                if (line[1] == '/') {
                    if(!tag.empty()){
                        if (line != ("</" + tag.top())) {
                            // unmatched closing and opening tags    --->   change the closing tag to be suitable with the opening tag
                            res[i] = "</" + tag.top();
                            tag.pop();
                            line_num.pop();
                        }
                        else {
                            tag.pop();
                            line_num.pop();
                        }
                    }
                    else{
                        // closing tag without opening tag    --->   remove it
                        res[i] = "D";
                    }
                }
                else if(line[1] != '!' && line[1] != '?') {
                    if(!(line[line.length()-2] == '/' && line[line.length()-1] == '>')){
                        QString temp = "";
                        for(int j=1; j<line.length(); j++){
                            if(line[j] != ' ' && line[j] != '>'){
                             temp += line[j];
                            }
                            else{
                                temp += '>';
                                break;
                            }
                        }
                        tag.push(temp);
                        line_num.push(i);
                    }
                }
            }
            else{
                // this line is a normal string
                // opening tag without closing tag and the tag value is just a string   --->   add the missing closing tag
                if(i+1 < xml.length()){
                    QString should_be_closing = xml[i+1];
                    if(!(should_be_closing[0] == '<' && should_be_closing[1] == '/')){
                        if(!tag.empty()){
                            QString correct = "</" + tag.top();
                            tag.pop();
                            line_num.pop();
                            res[i] = "U" + correct;
                        }
                    }
                }
            }
        }
        if(!tag.empty()){
            // opening tag without closing tag    --->   add the missing closing tag
            int size = tag.size();
            for(int j = 0; j<size; j++){
                QString extra = tag.top();
                res[line_num.top()] = "A" + extra;
                tag.pop();
                line_num.pop();
            }
        }
        return res;
}
