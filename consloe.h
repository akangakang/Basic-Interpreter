#ifndef CONSLOE_H
#define CONSLOE_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>

class consloe : public QTextEdit
{
    Q_OBJECT
public:
    explicit consloe(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // CONSLOE_H
