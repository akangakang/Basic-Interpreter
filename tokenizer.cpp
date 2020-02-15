#include "tokenizer.h"

Tokenizer::Tokenizer(QWidget *parent) : QWidget(parent)
{

}

int Tokenizer::getLineNum(QString newLine)
{
    std::string newline=newLine.toStdString();
    char * check = &newline[0];

    //吃掉前面空格
    while(*check==' ') check++;

    int num =0;


    if('0' <= *check  && *check <= '9')
    {
        while('0' <= *check  && *check <= '9')
        {
            num*=10;
            num += *check - '0';
            check++;
        }
        if(*check!=' ')
        {
            num=-2;//不合法行号
           wrongInput(1);
        }

    }
    else num=-1;

    //如果没有写行号或者行号为负（不是数字开头）则返回-1
    return num;
}

int Tokenizer::getState(QString newLine,QString *content)
{
    //basic不区分大小写 全转化为小写
    newLine.toLower();
    std::string newline=newLine.toStdString();


    char * check = &newline[0];

    //吃掉空格
    while(*check==' ') check++;

    //防止输入空行也报错
    if(*check =='\0') return 0;

    //吃掉行号
    while ('0' <= *check  && *check <= '9') check ++;
    while(*check==' ') check++;

    QString state;
    while('a' <= *check  && *check <= 'z')
    {
        state.append(*check);
        content->append(*check);
        check++;
    }

    while(*check != '\0')
    {
        content->append(*check);
        check++;
    }


    content->append('\0');


    if(state == "rem") return REM;
    if(state == "let") return LET;
    if(state == "print") return PRINT;
    if(state == "input") return INPUT;
    if(state == "goto") return GOTO;
    if(state == "if") return IF;
    if(state == "end") return END;
    if(state == "run")  return RUN;
    if(state == "list")  return LIST;
    if(state == "clear") return CLEAR;
    if(state == "help") return HELP;
    if(state == "quit") return QUIT;
    else
    {
      wrongInput(2);
       return -1;
    }


}

QString Tokenizer::getVarExp(QString Content,QString * exp)
{
    QString var;
    std::string content=Content.toStdString();
    char *check = &content[0];

    //吃掉空格
    while(*check==' ') check++;

    /*
      1、必须以字母开头；
      2、不可以包含嵌入的句号或者类型声明字符；
      3、不能和受到限制的关键字同名；
      4、由字母数字下划线组成
    */

    //必须以字母开头
    if(*check<'a' || *check>'z')
    {
        wrongInput(3);
        return " ";
    }


    while((*check >= 'a' && *check <= 'z')  || (*check >='0' && *check <='9') || *check=='_')
    {
        var.append(*check);
        check++;
    }

    while(*check==' ') check++;

    if(*check != '=')
    {
        wrongInput(4);
    }
    else {
        check++;
        while(*check==' ') check++;
        while(*check != '\0')
        {
            exp->append(*check);
            check++;
        }
    }

    return var;
}

QString Tokenizer::getVar(QString Content)
{
    QString var;
    std::string content=Content.toStdString();
    char *check = &content[0];

    //吃掉空格
    while(*check==' ') check++;

    /*
      1、必须以字母开头；
      2、不可以包含嵌入的句号或者类型声明字符；
      3、不能和受到限制的关键字同名；
      4、由字母数字下划线组成
    */

    //必须以字母开头
    if(*check<'a' || *check>'z')
    {
        wrongInput(4);
        return " ";
    }


    while((*check >= 'a' && *check <= 'z')  || (*check >='0' && *check <='9') || *check=='_')
    {
        var.append(*check);
        check++;
    }

    return var;

}
QString Tokenizer::removeKey(QString Content)
{
    std::string content=Content.toStdString();
    char *check = &content[0];

      while(*check==' ') check++;
      while ('0' <= *check  && *check <= '9') check ++;
      while(*check==' ') check++;

      QString exp;
      while('a' <= *check  && *check <= 'z')
      {
          check++;
      }

      while(*check != '\0')
      {
          exp.append(*check);
          check++;
      }

      return exp;
}

QString Tokenizer::getExp1(QString exp)
{
    QString  exp1;
    int i=0;
    while(exp[i].toLatin1()!='<' && exp[i].toLatin1()!='=' && exp[i].toLatin1()!='>'  )
    {
        exp1.append(exp[i]);
        i++;
    }
    return exp1;


}

QString Tokenizer::getExp2(QString exp)
{
    QString  exp2;
    int i=0;
    exp.toLower();
    while(exp[i].toLatin1()!='<' && exp[i].toLatin1()!='=' && exp[i].toLatin1()!='>'  )
    {
        i++;
    }
    i++;

    if(exp[i].toLatin1()!='<' && exp[i].toLatin1()!='=' && exp[i].toLatin1()!='>')
    {
        int idxOfThen=exp.indexOf("then");
        while(i<idxOfThen)
        {
            exp2.append(exp[i]);
            i++;
        }
    }


    return exp2;


}

QChar Tokenizer::getOp(QString exp)
{
    QChar op;
    int i=0;
    while(exp[i].toLatin1()!='<' && exp[i].toLatin1()!='=' && exp[i].toLatin1()!='>'  )
    {
        i++;
    }

    op=exp[i];

    //异常处理
    if(exp[i+1]=='<' || exp[i+1]=='=' || exp[i+1]=='>') return -1;
    if(!exp.contains('<') && !exp.contains('=') && !exp.contains('>')) return -1;

    return op;

}

int Tokenizer::getThenNum(QString exp)
{
    exp=exp.toLower();

    int i = exp.indexOf("then");


    int data=0;

    while(exp[i]<'0' || exp[i]>'9') i++;
    while(i<exp.length())
    {
        data=data*10+exp[i].toLatin1()-'0';
        i++;
    }
    return data;


}
