#ifndef BASICWINDOW_H
#define BASICWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QPixmap>
#include "console.h"
#include "tokenizer.h"
#include <QMap>
#include <QApplication>
#include "calc.h"
#include <QTextEdit>
#include <QApplication>

namespace Ui {
class BasicWindow;
}

class BasicWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum KEY{REM,LET,PRINT,INPUT,GOTO,IF,END,RUN,LIST,CLEAR,HELP,QUIT};
    explicit BasicWindow(QWidget *parent = nullptr);
    ~BasicWindow();


    QApplication *app;


    //界面
    console *inputWindow;
    console *outputWindow;
    QPushButton *name;


    //存代码
    QMap<int,QString> program;
    //存数据
    QMap<QString,QString> variable;

    //操作
    Tokenizer *check;
    calc *calcExp;
    bool inputFlag;
    QString inputVal;
    bool AllFlag;

     void run();
     void help();
     void clear();
     void list();
     void quit();
     int iff(QString);
     int gotoo(QString);
     void let(QString);
     void print(QString);
     void input(QString);

public slots:
    void receiveNewLine(QString newline);
     void receiveCheckVar(QString cVar);
    void wrong(int n);
    void receiveInput(QString ans, int flag);


private:
    static const int BASIC_WINDOW_WIDTH = 800;
    static const int BASIC_WINDOW_HEIGHT = 600;



private:
    Ui::BasicWindow *ui;
};

#endif // BASICWINDOW_H
