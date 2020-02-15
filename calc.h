#ifndef CALC_H
#define CALC_H

#include <QWidget>


class calc : public QWidget
{
    Q_OBJECT
public:
    explicit calc(QWidget *parent = nullptr);
private:
    enum Type {DATA ,ADD , SUB ,MULTI ,DIV,OPAREN,CPAREN,EOL,EP,ADD1,SUB1};

    struct node{
        Type type;
        int data;
        node *lchild ,*rchild;

        node (Type t ,int d=0 ,node *lc =NULL ,node *rc =NULL)
        {
            type = t;
            data =d ;
            lchild =lc;
            rchild =rc;

        }
    };

    node * root;

    node * create(char *&s);//从s创建一个表达式树
    Type getToken(char *&s ,int & value);
    int result (node *t);
    int negFlag=0;
    Type last=EOL;


public:
   // calc(char *s){root = create(s);}
    calc(int x){root=NULL; receiveVarVal=x;}
    int ini(char *s)
    {
        char*t=s;
        if(check(t))
        {
            root = create(s);
            return 1;
        }
        else {
            return 0;
        }
    }
    int result()
    {
        if(root == NULL) return 0;
        return result(root);
    }
    int receiveVarVal;
    int check(char *s);



signals:
      void checkVar(QString newline);

public slots:
};

#endif // CALC_H
