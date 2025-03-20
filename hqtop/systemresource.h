#ifndef SYSTEMRESOURCE_H
#define SYSTEMRESOURCE_H

#include <QObject>

class SystemResource : public QObject
{
    Q_OBJECT
public:
    explicit SystemResource(QObject *parent = nullptr);

signals:

};

#endif // SYSTEMRESOURCE_H
