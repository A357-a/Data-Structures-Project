#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xml_vector.h"
#include "consistency_check.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "json.h"
#include "compression.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
     QString filter = "Text File (*.txt) ;; XML File (*.xml)";
    QString file_name = QFileDialog::getOpenFileName(this,"Choose The XML File",QDir::homePath(),filter);
    QFile file (file_name);
    if(!file.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","file not open");
    }
    QTextStream in (&file);
    QString text = in.readAll();
    ui->plainTextEdit->setPlainText(text);
    file.close();
}


void MainWindow::on_pushButton_2_clicked()
{
    QString text = ui->plainTextEdit->toPlainText();
    bool message;
    message = check_consistency(create_xml_vector(text));
    if(message == true){
            QMessageBox::information(this, "Info", "This XML file is consistent");
        }
        else{
            QMessageBox::warning(this, "Warning", "This XML file is NOT consistent");
            return;
        }
}



void MainWindow::on_pushButton_3_clicked()
{

    QString in = ui->plainTextEdit->toPlainText();
    if(in == ""){
        QMessageBox::warning(this, "Warning", "No Text To Be Converted");
        return;
    }
    /*
    // File input
    QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the JSON File", QDir::currentPath());
    fpath = filename;
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save File!");
        return;
    }
    */


    // Processing



    if(!check_consistency(create_xml_vector(in))){
        QMessageBox::warning(this, "Warning", "Cannot Convert An Inconsistent XML File");
        return;
    }
    string inq = in.toLocal8Bit().constData();
    vector<string> linesQ = create_xml_vectorQ(inq);
    int linesQSize = linesQ.size();
    node *root;
    root = create_tree(linesQ, linesQSize);
    string out = "";
    out  = iteration(root, out);
    QString res = QString::fromStdString(out);



    /*
    for(int i=0; i<linesQ.size(); i++){
        cout << linesQ[i] << '\n';
    }
    */
    // Output
    //--------------
    // on screen
    ui->textEdit->setText(res);

    // to file
    QString fname = QFileDialog::getSaveFileName(this, "Choose the Location to Save the JSON File", QDir::currentPath());
    QFile fileQ(fname);
    if(!fileQ.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Save File!");
        return;
    }
    QTextStream outt(&fileQ);
    outt << res;
    fileQ.flush();
    fileQ.close();
    QMessageBox::information(this, "Info", "File Coverted Successfully!\nThe JSON File Can Be Found at: " + fname);
    // read the file after the output
ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
    QFile inn(fname);
    if(!fileQ.open(QFile::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
        return;
    }
    QTextStream inp(&inn);
    QString text_in_area;
    if(inn.size() > 3*1024*1024){
        ui->textEdit->setText("");
        QMessageBox::warning(this, "Warning", "Due to large file size > 3 MB, we will view the first 100 lines of the output file only");
        for(int q=0; q<100; q++){
            text_in_area = inp.readLine();
            ui->textEdit->append(text_in_area);
        }
    }
    else{
        text_in_area = inp.readAll();
        ui->textEdit->setText(text_in_area);
    }
    ui->textEdit->setText(res);

    inn.close();
}

void MainWindow::on_pushButton_4_clicked()
{
    int sizeee = 0;
    map<char, string> new_cod;
    ui->statusbar->showMessage("");
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        // declaring vars
        queue<unsigned char> ag;
        double old_sizeee = 0;
        double new_sizeee = 0;
        // Handeling opening
        QString filename = QFileDialog::getOpenFileName(this, "Choose a File", QDir::currentPath());
        QFile old(filename);
        if(!old.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot Open File!");
            return;
        }
        QTextStream inOld(&old);
        QString cQ = inOld.readAll();
        string n = cQ.toLocal8Bit().constData();
        // calculating old size
        old_sizeee = old.size()/1024.0;
        old.close();

        QString fname = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Compressed File", QDir::currentPath());
        QFile neew(fname);
        if(!neew.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot Save File!");
            return;
        }
        // Choosing the location to save the compression key

        QString codeName = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Compression Key File", QDir::currentPath());
        QFile key(codeName);
        if(!key.open(QFile::WriteOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot Save File!");
            return;
        }
        fstream write_to_compressed;
        fstream write_to_key;
        write_to_compressed.open(fname.toLocal8Bit().constData(), ios::out);
        write_to_key.open(codeName.toLocal8Bit().constData(), ios::out);

        tree nm;
        ag = nm.maketree(n, new_cod, sizeee);
        unsigned char as;
        string final = "";
        for (unsigned int i = 0; ag.size() != 0; i++) {
            as = ag.front();
            final += as;
            ag.pop();
        }
        write_to_compressed << final;
        neew.flush();
        neew.close();
        // writing the compression key code file
        write_to_key << sizeee << endl;
        for(auto i = new_cod.begin(); i!=new_cod.end(); i++){
            string temp = i->second + '&' + i->first;
            write_to_key << temp;
        }
        write_to_compressed.close();
        write_to_key.close();
        // Finalization
        ui->statusbar->showMessage("Done!");
        QFile file(fname);
        if(!file.open(QFile::ReadOnly | QFile::Text)){
            QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
            return;
        }
        else{
            new_sizeee = file.size()/1024.0;
            QMessageBox::information(this, "Info", "File Compressed Successfully!\nThe Compressed File Can Be Found At: " + fname + "\nOld File Size is " + QString::number(old_sizeee) + " KB\nNew File Size is " + QString::number(new_sizeee) + " KB");
            QTextStream in(&file);
            QString file_text = "";
            if(new_sizeee > 3*1024){ // file larger than 3 MB
                ui->textEdit->setText("");
                QMessageBox::warning(this, "Warning", "Due to large file size > 3 MB, we will view the first 100 lines of the output file only");
                for(int m=0; m<100; m++){
                    file_text = in.readLine();
                    ui->textEdit->append(file_text);
                }
            }
            else{
                file_text = in.readAll();
                ui->textEdit->setText(file_text);
            }
            file.close();
            ui->statusbar->showMessage("Done!");
}
}

void MainWindow::on_pushButton_5_clicked()
{
    QVector <QString> lines;
    ui->statusbar->showMessage("");
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        //-----------------------------------------------------------------------------
        if(lines.size() == 0){
            lines = create_xml_vector(ui->plainTextEdit->toPlainText());
        }
        QVector<QString> temp = lines;
        QMap<qint32, QString> errors = identify_errors(temp);
        ui->textEdit->setText("");
        int i = 0;
        for(i=0; i<temp.size(); i++){
            if(errors.find(i) != errors.end()){
                ui->textEdit->setTextBackgroundColor(Qt::red);
                ui->textEdit->append(temp[i]);
                ui->textEdit->setTextBackgroundColor(Qt::transparent);
            }
            else{
                ui->textEdit->append(temp[i]);
            }
        }
        lines = temp;
        //-----------------------------------------------------------------------------
        ui->statusbar->showMessage("Done!     Number of Errors: " + QString::number(errors.size()));
}


void MainWindow::on_pushButton_6_clicked()
{
    ui->statusbar->showMessage("");
        ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        //-----------------------------------------------------------------------------
        QVector <QString> lines;
        QString out = "";
        if(lines.empty()){
            QString in = ui->plainTextEdit->toPlainText();
            if(in != ""){
                QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Minified File", QDir::currentPath());
                QFile file(filename);
                if(!file.open(QFile::WriteOnly | QFile::Text)){
                    QMessageBox::warning(this, "Warning", "Cannot Save File!");
                    return;
                }
                lines = create_xml_vector(in);
                for(int i=0; i<lines.size(); i++){
                    if(!(lines[i][0] == '<' && lines[i][1] == '!')){
                        out += lines[i];
                    }
                }
                QTextStream outt(&file);
                outt << out;
                file.flush();
                file.close();
                QMessageBox::information(this, "Info", "File Minified Successfully!\nThe Minified File Can Be Found at: " + filename);
                QFile f(filename);
                if(!f.open(QFile::ReadOnly | QFile::Text)){
                    QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
                    return;
                }
                else{
                    QTextStream in(&f);
                    QString file_text = "";
                    file_text = in.readAll();
                    ui->textEdit->setText(file_text);
                    f.close();
                    ui->statusbar->showMessage("Done!");
                }
            }
            else{
                QMessageBox::warning(this, "Warning", "No Text To Be Minified!");
                return;
            }
        }
        else{
            QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Minified File", QDir::currentPath());
            QFile file(filename);
            if(!file.open(QFile::WriteOnly | QFile::Text)){
                QMessageBox::warning(this, "Warning", "Cannot Save File!");
                return;
            }
            for(int i=0; i<lines.size(); i++){
                if(!(lines[i][0] == '<' && lines[i][1] == '!')){
                    out += lines[i];
                }
            }
            QTextStream outt(&file);
            outt << out;
            file.flush();
            file.close();
            QMessageBox::information(this, "Info", "File Minified Successfully!\nThe Minified File Can Be Found at: " + filename);
            QFile f(filename);
            if(!f.open(QFile::ReadOnly | QFile::Text)){
                QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
                return;
            }
            else{
                QTextStream in(&f);
                QString file_text = "";
                file_text = in.readAll();
                ui->textEdit->setText(file_text);
                f.close();
                ui->statusbar->showMessage("Done!");
            }
        }
        ui->textEdit->setText(out);

}


void MainWindow::on_pushButton_7_clicked()
{
    ui->statusbar->showMessage("");
       ui->textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
       //-----------------------------------------------------------------------------
       QVector <QString> lines;
       QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Consistent File", QDir::currentPath());

       QFile file(filename);
       if(!file.open(QFile::WriteOnly | QFile::Text)){
           QMessageBox::warning(this, "Warning", "Cannot Save File!");
           return;
       }
       if(lines.size() == 0){
           lines = create_xml_vector(ui->plainTextEdit->toPlainText());
       }
       QVector<QString> temp = lines;
       QString consistent = "";
       QMap<qint32, QString> errors = identify_errors(temp);
       ui->textEdit->setText("");
       int i = 0;
       for(i=0; i<temp.size(); i++){
           if(errors.find(i) != errors.end()){
               if(errors[i][0] == 'A'){
                   consistent += temp[i];
                   consistent += '\n';
               }
               else if(errors[i][0] == 'U'){
                   QString value = errors[i];
                   QString viewValue = "";
                   for(int k = 1; k < value.length(); k++){
                       viewValue += value[k];
                   }
                   consistent += temp[i];
                   consistent += '\n';
                   consistent += viewValue;
                   consistent += '\n';
               }
               else if(errors[i][0] != 'D'){
                   consistent += errors[i];
                   consistent += '\n';
               }
           }
           else{
               consistent += temp[i];
               consistent += '\n';
           }
       }

       QVector<int> line_num;
       for(auto j = errors.begin(); j != errors.end(); j++){
           QString value = j.value();
           if(value[0] == 'A'){
               line_num.push_back(j.key());
           }
       }
       for(int q = line_num.length()-1; q >= 0; q--){
           QString value = errors[line_num[q]];
           QString viewValue = "";
           for(int k = 1; k < value.length(); k++){
               viewValue += value[k];
           }
           viewValue = "</" + viewValue;
           consistent += viewValue;
           consistent += '\n';
       }
       //-----------------------------------------------------------------------------
       QTextStream out(&file);
       out << consistent;
       file.flush();
       file.close();
       QMessageBox::information(this, "Info", "File Fixed Successfully!\nThe Consistent File Can Be Found at: " + filename);
       QFile f(filename);
       if(!f.open(QFile::ReadOnly | QFile::Text)){
           QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
           return;
       }
       else{
           QTextStream in(&f);
           QString file_text = "";
           if(f.size() > 3*1024*1024){ // file larger than 3 MB
               ui->textEdit->setText("");
               QMessageBox::warning(this, "Warning", "Due to large file size > 3 MB, we will view the first 100 lines of the output file only");
               for(int q=0; q<100; q++){
                   file_text = in.readLine();
                   ui->textEdit->append(file_text);
               }
           }
           else{
               file_text = in.readAll();
               ui->textEdit->setText(file_text);
           }
           f.close();
           ui->statusbar->showMessage("Done!");
       }
}
QString remove_one_indentation(QString str, QString indent){
    QString temp = "";
    qint32 indLen = indent.length();
    for(int i=0; i<str.length()-indLen;i++){
        temp += str[i];
    }
    return temp;
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->statusbar->showMessage("");
       //-----------------------------------------------------------------------------
    QVector <QString> lines;
    QString indent_char = "     ";
       if(lines.size() == 0){
           lines = create_xml_vector(ui->plainTextEdit->toPlainText());
       }
       if(lines.size() == 0){
           QMessageBox::warning(this, "Warning", "No text to be prettified!");
           return;
       }
       QString filename = QFileDialog::getSaveFileName(this, "Choose the Location to Save the Prettified File", QDir::currentPath());
       QFile file(filename);
       if(!file.open(QFile::WriteOnly | QFile::Text)){
           QMessageBox::warning(this, "Warning", "Cannot Save File!");
           return;
       }
       if(check_consistency(lines)){
           QString indent = "";
           QString final = "";
           for(int i=0; i<lines.size(); i++){
              QString line = lines[i];
              if(line[0] == '<'){
                  if(line[1] == '/'){
                       indent = remove_one_indentation(indent, indent_char);
                       final += indent+line;
                       final += "\n";
                  }
                  else if(line[1] != '!' && line[1] != '?'){
                      if(!(line[line.length()-2] == '/' && line[line.length()-1] == '>')){
                          final += indent+line;
                          final += "\n";
                          indent += indent_char;
                      }
                      else{
                          final += indent+line;
                          final += "\n";
                      }
                  }
                  else{
                      final += indent+line;
                      final += "\n";
                  }
              }
              else{
                  final += indent+line;
                  final += "\n";
              }
           }
           //ui->textEdit->setText(final);
           QTextStream out(&file);
           out << final;
           file.flush();
           file.close();
           QMessageBox::information(this, "Info", "File Prettified Successfully!\nThe Prettified File Can Be Found at: " + filename);
           ui->textEdit->setLineWrapMode(QTextEdit::NoWrap);
           QFile f(filename);
           if(!f.open(QFile::ReadOnly | QFile::Text)){
               QMessageBox::warning(this, "Warning", "Cannot Open the Results File!");
               return;
           }
           else{
               QTextStream in(&f);
               QString file_text = "";
               if(f.size() > 3*1024*1024){ // file larger than 3 MB
                   ui->textEdit->setText("");
                   QMessageBox::warning(this, "Warning", "Due to large file size > 3 MB, we will view the first 100 lines of the output file only");
                   for(int q=0; q<100; q++){
                       file_text = in.readLine();
                       ui->textEdit->append(file_text);
                   }
               }
               else{
                   file_text = in.readAll();
                   ui->textEdit->setText(file_text);
               }
               f.close();
               ui->statusbar->showMessage("Done!");
           }

       }
       else{
           QMessageBox::warning(this, "Warning", "Cannot Prettify an Inconsistent XML file!");
           return;
       }
}




