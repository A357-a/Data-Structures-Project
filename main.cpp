#include "mainwindow.h"

#include <QApplication>
using namespace std;

unsigned int NumOfLines = 0;
unsigned int sizeOfXML =0;
unsigned int sizeOfJSON =0;

vector<string> XML_ReadFile;
vector<string> XML_original;
vector<string> XML_FixedErrors;
vector<string> Spaces;
vector<string> JSON;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


