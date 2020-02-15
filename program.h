#ifndef PROGRAM_H
#define PROGRAM_H

#include <QWidget>
#include <QMap>

class Program : public QWidget
{
    Q_OBJECT
public:
    explicit Program(QWidget *parent = nullptr);
    QMap<int , QString> program;

signals:

public slots:
};

#endif // PROGRAM_H
