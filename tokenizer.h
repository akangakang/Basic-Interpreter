#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <QWidget>


class Tokenizer : public QWidget
{
    Q_OBJECT
public:
    explicit Tokenizer(QWidget *parent = nullptr);
    enum KEY{REM,LET,PRINT,INPUT,GOTO,IF,END,RUN,LIST,CLEAR,HELP,QUIT};
    int getLineNum(QString);
    int getState(QString,QString *);
    QString getVarExp(QString,QString *);
    QString getVar(QString);
    QString removeKey(QString);//输入只有变量 用于input

    QString getExp1(QString);
    QString getExp2(QString);
    QChar getOp(QString);
    int getThenNum(QString);
signals:
    void wrongInput(int n);

public slots:
};

#endif // TOKENIZER_H
