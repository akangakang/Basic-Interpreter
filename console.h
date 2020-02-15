#ifndef CONSOLE_H
#define CONSOLE_H

#include <QWidget>
#include <QTextEdit>
#include <program.h>

class console : public QTextEdit
{
    Q_OBJECT
public:
    explicit console(QWidget *parent = nullptr);


signals:
    void newLineWritten(QString newline);
    void newInputVal(QString val,int flag);

public slots:

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

};

#endif // CONSOLE_H
