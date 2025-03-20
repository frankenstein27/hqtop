#ifndef PROCESSINFO_H
#define PROCESSINFO_H

#include <QObject>

class ProcessInfo : public QObject
{
    Q_OBJECT
public:
    explicit ProcessInfo(QObject *parent = nullptr);

signals:

};

#endif // PROCESSINFO_H
