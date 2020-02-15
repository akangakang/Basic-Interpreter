#include "basicwindow.h"
#include "ui_basicwindow.h"

BasicWindow::BasicWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BasicWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("BASIC");
    this->setFixedSize(BASIC_WINDOW_WIDTH, BASIC_WINDOW_HEIGHT);
    this->setStyleSheet("background-color: rgb(72,72,72)");

    inputWindow = new console (this);
    outputWindow = new console (this);
    check =new Tokenizer (this);



    inputWindow->setGeometry(0,0,700,450);
    inputWindow->setStyleSheet("background-color: rgb(41,41,41);border :8px solid rgb(72,72,72) ");
    inputWindow->setTextColor("white");
    inputWindow->setFont(QFont("Consolas"));



    outputWindow->setGeometry(0,447,700,150);
    outputWindow->setStyleSheet("background-color: rgb(41,41,41);border :8px solid rgb(72,72,72) ");
    outputWindow->setTextColor("red");
    outputWindow->setFont(QFont("Consolas"));



   // inputWindow->append("REM hello world!");




    inputFlag=1;
    inputVal="0";

    QFont nameFont;
    nameFont.setPointSize(20);
    nameFont.setFamily("Bradley Hand ITC");
    nameFont.setBold(1);
    name = new QPushButton (this);
    name -> setGeometry(720,170,50,280);
    name -> setText("B\nA\nS\nI\nC");
    name -> setFont(nameFont);
    name -> setStyleSheet("color : rgb(200,200,200) ");
    name -> setFlat(1);

    connect(inputWindow,&console::newLineWritten,this,&BasicWindow::receiveNewLine);
    connect(check,&Tokenizer::wrongInput,this,&BasicWindow::wrong);

    calcExp =new calc(0);

   connect(calcExp,SIGNAL(checkVar(QString)),this,SLOT(receiveCheckVar(QString)));
    connect(inputWindow,&console::newInputVal,this,&BasicWindow::receiveInput);

}

BasicWindow::~BasicWindow()
{
    delete ui;
}

void BasicWindow::receiveNewLine(QString newline)
{

    AllFlag=1;
    int num = check->getLineNum(newline);//得到行号 无行号则num为-1 不合法行号为-2
    bool flag = true; //判断要不要把这行写到program里面去
    QString content;
    int state=-1 ;
    state = check -> getState(newline,&content);
    newline.toLower();

    if(num==-1 && (state==-1 || state==4 || state==5 || state==6))
    {
        outputWindow->append("Key word error! (line number needed)");
    }
    if(num>=0 && (state==0 || state==7 ||state==8 ||state==9 ||state==10 || state==11 ))
    {
        flag=false;
        outputWindow->append("Key word error! (no line number needed)");
    }
    if(state==IF)
    {
        if(!newline.contains("then")) outputWindow->append("key word error! (if...then)");
    }




    //没有行号的就不写进去
    if( num < 0) flag=false;



    //检查是否是立即执行的关键字 是的话则立即执行
    switch(state)
    {

    //必须立即执行
    case RUN:
        flag = false ;
        try{
        run();

    }
        catch(QString msg)
    {


            outputWindow->append(msg); break;

    }
        break;

    case LIST:
        flag = false ;
        list();
        break;

    case CLEAR:
         clear();
        break;

    case HELP:
         help();
        break;

    case QUIT:
        quit();
        break;

    case LET:
        if(num!=-1)//有行号
       break;
        else {
            //没有行号
             QString exp=check->removeKey(content);
             try{  let(exp);}
             catch(QString msg)
         {


                 outputWindow->append(msg); break;

         }
             break;


        }

    case PRINT:
        if(num!=-1)//有行号
       break;
        else {
            //没有行号
             QString exp=check->removeKey(content);
            try{print(exp);}
            catch(QString msg)
        {


                outputWindow->append(msg); break;

        }
             break;

        }

    case INPUT:
        if(num!=-1)//有行号
       break;
        else {
            //没有行号
             QString exp=check->removeKey(content);

             input(exp);
             break;

        }

    case REM:
        flag=0;



    }

    //如果该行程序正确 并且右行号 才写入program
    if (flag && AllFlag)
    {
        program.insert(num,content);
    }

}

void BasicWindow::wrong(int n)
{
    /*
     * 1==输入第一个不是数字也不是statement
     */
    AllFlag=0;

    switch(n)
    {
    case 1:
        outputWindow->append("Line number error!");
        break;
    case 2:
        outputWindow->append("Key word error!");
        break;
    case 3:
        outputWindow->append("ERROR : invalid variable ");
        break;
    case 4:
        outputWindow->append("ERROR: There must be a euqal sign after variable");
        break;

    }
}


void BasicWindow::run()
{
     QMap<int,QString>::iterator it;
     //不如先清空一下变量
     variable.clear();
     //inputWindow->append("\n");
     for ( it = program.begin(); it != program.end(); ++it )
     {
         QString content;
         int state=-1 ;
         state = check -> getState(it.value(),&content);

         //现在的content是带关键字的 先去掉关键字

         QString exp=check->removeKey(content);
         //exp是没有关键字的
         switch (state) {
         case LET:

             let(exp);


             break;


         case PRINT:
             print(exp);
             break;

         case INPUT:

             input(exp);
             break;

         case IF:

             //如果不包含then报错
         {
             int op=iff(exp);
             QString error;
             QString line;
             int lineNum =check->getThenNum(exp);

             if(program.contains(lineNum) && op==1)
             {
                 it=program.find(lineNum);
                 it--;
             }

             //then后行号错误
             else if(!program.contains(lineNum))
             {
                  int frolinenum=it.key();
                 error.append("ERROR: LINE ");
                 if(frolinenum==0) line.append("0");
                 while(frolinenum>0)
                 {
                     line.insert(0,'0'+frolinenum%10);
                     frolinenum/=10;
                 }
                 error.append(line);
                 error.append(" : invalid line number after THEN");
                 //outputWindow->append(error);
                 throw error;

             }
             //比较符错误
             if(op==-1)
             {
                 int frolinenum=it.key();
                error.append("ERROR: LINE ");
                if(frolinenum==0) line.append("0");
                while(frolinenum>0)
                {
                    line.insert(0,'0'+frolinenum%10);
                    frolinenum/=10;
                }
                error.append(line);
                error.append(" : no valid operator");
               // outputWindow->append(error);
                throw error;

             }


             break;
         }
         case GOTO:
         {
             int nextKey =gotoo(exp);
             if(program.contains(nextKey))
             {
                 it=program.find(nextKey);
                 it--;
             }
             else {
                 QString error;
                 QString line;
                 int frolinenum=it.key();
                 error.append("ERROR: LINE ");
                 if(frolinenum==0) line.append("0");
                 while(frolinenum>0)
                 {
                     line.insert(0,'0'+frolinenum%10);
                     frolinenum/=10;
                 }
                 error.append(line);
                 error.append(" : invalid line number after GOTO");
                // outputWindow->append(error);
                 throw error;

             }

             break;
         }

         case END:
             it = program.end();
             it--;
             //还要把变量清空
             variable.clear();
             break;


     }


     }

}

int BasicWindow::iff(QString exp)
{
    QString exp1,exp2;
    QChar op;
    exp1=check->getExp1(exp);
    exp2=check->getExp2(exp);

    std::string expression =exp1.toStdString();

    int val1,val2;
    if(calcExp->ini(&expression[0]))
    {
         val1=calcExp->result();

    }
    else
    {
        val1=0;
        outputWindow->append("ERROR : wrong expression");
    }

     expression =exp2.toStdString();
    if(calcExp->ini(&expression[0]))
    {
         val2=calcExp->result();

    }
    else
    {
        val2=0;
        outputWindow->append("ERROR : wrong expression");
    }




    op=check->getOp(exp);
    char opchar=op.toLatin1();

    switch (opchar)
    {
    case '<':
        if(val1<val2) return 1; else return 0;

    case '=':
        if(val1==val2) return 1;else return 0;

    case '>':
        if(val1>val2)  return 1;else return 0;
    default:
        return -1;


    }



}
void BasicWindow::input(QString content)
{
    QString var =check->getVar(content);

    //inputWindow->insertPlainText("\n");


    inputWindow->append(" ?  ");

    inputFlag=1;
    inputVal="0";

    while(inputFlag) {QCoreApplication::processEvents();}

    if(variable.contains(var))
        {
                variable[var]=inputVal;
        }
        else {

            variable.insert(var, inputVal);
        }
}

void BasicWindow::receiveInput(QString ans,int flag)
{

    inputVal=ans;
    inputFlag=0;

}
void BasicWindow::print(QString content)
{
    //print x=1类型
    if(content.contains("="))
    {
        QString exp;
        QString var;
        var = check->getVarExp(content,&exp);
        std::string expression =exp.toStdString();
        int ans;
        if(calcExp->ini(&expression[0]))
        {
             ans=calcExp->result();

        }
        else
        {
            ans=0;
            outputWindow->append("ERROR : wrong expression");
        }
        QString numStr;
        if(variable.contains(var))
        {
                variable[var].setNum(ans);
        }
        else {

            numStr.setNum(ans);
            variable.insert(var, numStr);
        }


        inputWindow->append(numStr);

    }
    //print x/1/x+1类型
    else {
        std::string expression =content.toStdString();

        int ans;
        if(calcExp->ini(&expression[0]))
        {
             ans=calcExp->result();

        }
        else
        {
            ans=0;
            outputWindow->append("ERROR : wrong expression");
        }
        ans=calcExp->result();
        QString numStr;

        numStr.setNum(ans);
        inputWindow->append(numStr);

    }

}
void  BasicWindow::let(QString content)
{

    QString exp;
    QString var;
    var = check->getVarExp(content,&exp);
    std::string expression =exp.toStdString();

    int ans;
    if(calcExp->ini(&expression[0]))
    {
         ans=calcExp->result();

    }
    else
    {
        ans=0;
        outputWindow->append("ERROR : wrong expression");
    }


    if(variable.contains(var))
    {
            variable[var].setNum(ans);
    }
    else {
        QString numStr;
        numStr.setNum(ans);
        variable.insert(var, numStr);
    }




}

void BasicWindow::help()
{
    inputWindow -> append("变量名要求：");
    inputWindow -> append("1、必须以字母开头 不区分大小写；\n 2、不可以包含嵌入的句号或者类型声明字符；\n 3、不能和受到限制的关键字同名； \n 4、由字母数字下划线组成");

    inputWindow -> append("关键字要求");
    inputWindow -> append("REM,LET,PRINT,INPUT,GOTO,IF,END,RUN,LIST,CLEAR,HELP,QUIT");


}

void BasicWindow::list()
{
    QMap<int,QString>::iterator it;

    //inputWindow->append("\n");
    for ( it = program.begin(); it != program.end(); ++it )
    {
        QString  content;
        content.setNum(it.key());
        //content.append(it.key());
        content.append(" ");
        content.append(it.value());

        inputWindow->append(content);

    }

}

void BasicWindow::clear()
{
    inputWindow -> clear();

    //clear关键字应该也要清空内存
    program.clear();
    variable.clear();

}

void BasicWindow::quit()
{
     app->exit();

}

int BasicWindow::gotoo(QString exp)
{
    int i=0;
    while(exp[i]==' ') i++;
    int data=0;
    if(exp[i]>='0' && exp[i]<='9')
    {
        while(exp[i]>='0' && exp[i]<='9')
        {
            data=data*10+exp[i].toLatin1()-'0';
            i++;
        }
        return data;

    }

    else return -1;
}

void BasicWindow::receiveCheckVar(QString cVar)
{
    int val=0;
    int flag=0;
    if(variable.contains(cVar))
    {
        val = variable[cVar].toInt();
        flag=1;
    }
   calcExp->receiveVarVal=flag*val;
   if(!flag) outputWindow->append("ERROR:invalid variable");


}

