#include "console.h"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>

console::console(QWidget *parent) : QTextEdit(parent)
{

}
void console::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Backspace)
    {

    }
    if (event->key() == Qt::Key_Delete)
    {

    }

    if (this->textCursor().hasSelection())
        return;

    //回车存入program
    if (event->key() == Qt::Key_Return) {


        QTextCursor cursor = this->textCursor();

       cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
       // newLineWritten(lastLine);

        if(lastLine.contains('?') && !(lastLine.contains("print")) && !(lastLine.contains("rem")) && !(lastLine.contains("let")))
        {

            int i=0;
            int data=0;
            QString ddata;
             while((lastLine[i]<'0' || lastLine[i]>'9') && i<lastLine.length())
             {
                 i++;
             }
             if((lastLine[i]>='0' && lastLine[i]<='9') && i<lastLine.length())
             {
                 while((lastLine[i]>='0' && lastLine[i]<='9') && i<lastLine.length())
                 {
                      data=data*10+lastLine[i].toLatin1()-'0';
                      ddata.append(lastLine[i]);
                      i++;
                 }
             }

             int flag=0;
            newInputVal(ddata,flag);
            //newInputVal(lastLine,1);

        }
        else {
            newLineWritten(lastLine);
        }

     }
    QTextEdit::keyPressEvent(event);
}


