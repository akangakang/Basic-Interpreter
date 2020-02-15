#include "calc.h"

calc::calc(QWidget *parent) : QWidget(parent)
{

}

calc::node* calc::create(char *&s)
{
    calc::node *p,*root =NULL,*tmp;
    Type returnType;
    int value;
    bool flag=1;


    last = EOL;
    while(*s)
    {
        returnType = calc::getToken(s,value);
        switch(returnType)
        {
        case DATA:case OPAREN:
            if(returnType == DATA) p = new calc::node(DATA,value);
            else p = create(s);//遇到括号 递归
            if (p==NULL )
            {
                QString msg="ERROR : wrong expression!";
                delete p;
                throw msg;
            }
            if(root!= NULL)
                if(root->rchild == NULL) root -> rchild =p;
                else
                {
                    tmp=root;
                    while(tmp->rchild!=NULL)
                    {
                        tmp=tmp->rchild;
                    }

                    tmp->rchild=p;
                }
            else {
                if(p->type==ADD) p->type=ADD1;
                else if(p->type==SUB) p->type=SUB1;

                    root=p;



            }


            break;

        case CPAREN:case EOL: return root;

        case ADD:case SUB:
            if ( root == NULL)
            {
                p=new node(DATA,0);
                root = new node(returnType, 0,p);

            }
            else root = new node (returnType,0 , root); //原来的树作为左儿子
            break;

        case MULTI: case DIV:
            if ( root == NULL)
            {
               // p=new node(DATA,0);
                //root = new node(returnType, 0);
                QString msg="ERROR : wrong expression!";

                throw msg;

            }
            else if (root -> type == MULTI || root -> type == DIV)
            {
                root = new node (returnType,0 , root);
            }
            else if(root -> type == ADD || root -> type == SUB) root -> rchild = new node (returnType ,0 , root->rchild);
            else if(root->type ==ADD1 || root->type==SUB1) root=new node (returnType,0,root);
            else {
                root = new node (returnType,0 , root);
            }
            break;

        case EP:
            if( root == NULL)
            {
                QString msg="ERROR : wrong expression!";

                throw msg;
            }
            else if(root->type==DATA)
            {
                root=new node(returnType,0,root);
            }
            else
            {
               p=root;
               while(p->rchild->type!=DATA)
               {
                   p=p->rchild;
               }
               p->rchild=new node(returnType,0,p->rchild);

            }
            break;




         }

        if(!flag) break;
    }


    return root;


}

calc::Type calc::getToken(char *&s, int &data)
{
    char type;

    while(*s ==' ') ++s;
    data=0;

    //数字
    if(*s>='0' && *s<='9')
    {

        //考虑是变量的情况

        while(*s >= '0'  && *s<='9')
        {
            data = data * 10 + *s-'0';
            ++s;
        }

        if(last==DATA)
        {
            QString msg="ERROR : wrong expression!";

          throw msg;
        }
        last=DATA;

        return DATA;

    }

    //结束
    if(*s == '\0')  return EOL;


    //变量
    if((*s>='a'  && *s<='z') || (*s >= 'A' && *s<='Z') )
    {
        QString var;
        while((*s >= 'a'  && *s <= 'z') || (*s >= 'A' && *s<='Z') || (*s >= '0'  && *s<='9') || (*s=='_'))
        {
            var.append(*s);
            s++;
        }
        var.toLower();

        checkVar(var);
        data=receiveVarVal;


        if(last==DATA)
        {
            QString msg="ERROR : wrong expression!";

            throw msg;
        }
        last=DATA;
        return DATA;

    }

    //运算符
    type = *s;
    switch(type)
    {
    case '+':
        ++s;

        if(last!=DATA && last!=CPAREN)
        {
            QString msg="ERROR : wrong expression!";
            throw msg;
        }
        last=ADD;
        return ADD;
    case '-':
    {
        if(negFlag==0)
        {
            ++s;

            if(last!=DATA && last!=CPAREN && last!=EOL)
            {
                QString msg="ERROR : wrong expression!";
                throw msg;
            }
            last = SUB;
            return SUB;
        }
        else {
            s++;
            while(*s ==' ') ++s; data=0;
            if(*s>='0' && *s<='9')
            {

                while(*s >= '0'  && *s<='9')
                {
                    data = data * 10 + *s-'0';
                    ++s;
                }

               data=-data;
               negFlag=0;

               if(last==DATA)
               {
                   QString msg="ERROR : wrong expression!";
                   throw msg;
               }
               last=DATA;
               return DATA;

            }
        }

    }
    case '*':
        ++s;
        if(*s=='*')
        {
            ++s;

            if(last!=DATA && last!=CPAREN)
            {
                QString msg="ERROR : wrong expression!";
                throw msg;
            }
            last=EP;
            return EP;
        }
         else
        {

            if(last!=DATA && last!=CPAREN)
            {
                QString msg="ERROR : wrong expression!";
                throw msg;
            }
            last =MULTI;
            return MULTI;
        }
    case '/':
        ++s;

        if(last!=DATA && last!=CPAREN)
        {
            QString msg="ERROR : wrong expression!";
            throw msg;
        }
        last = DIV;
        return DIV;
    case '(':
    {
        char *t=s+1;
        while(*t==' ')t++;
        if(*t=='-')
            negFlag=1;
        ++s;last = OPAREN;return OPAREN;
    }
    case ')':  ++s;last = CPAREN;return CPAREN;
    default: return EOL;

    }


}

int calc::result(node *t)
{
    int num1,num2;

    if(t->type == DATA) return t->data;
    num1 =result(t->lchild);
    num2 =result(t->rchild);

    switch (t->type) {
    case ADD:case ADD1:
        t->data =num1 + num2;
        break;

    case SUB:case SUB1:
        t->data =num1 -num2;
        break;

    case MULTI:
        t->data =num1 * num2;
        break;

    case DIV:
        t->data =num1 / num2;
        break;
    case EP:
        int rnum=num2,ans=1;
        while(rnum!=0)
        {
            ans*=num1;
            rnum--;
        }
        t->data=ans;

    }

    return t->data;
}

int calc::check(char *s)
{
    while (*s)
    {
        if((*s>='a' && *s<='z') ||(*s>='0' && *s<='9') ||(*s>='A' && *s<='Z') ||(*s>='<' && *s<='>')  || (*s>='(' && *s<='+') || *s=='-' || *s=='_' ||*s=='/'||*s==' ' )
        {
            s++;
            continue;
        }
        else
        {

                QString msg="ERROR : wrong expression!";
                throw msg;


        }
    }
    return 1;


}
