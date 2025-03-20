#ifndef SYSTEMDATAPROVIDER_H
#define SYSTEMDATAPROVIDER_H

#include <QObject>

class SystemDataProvider : public QObject
{
    Q_OBJECT
public:
    explicit SystemDataProvider(QObject *parent = nullptr);

signals:

};

#endif // SYSTEMDATAPROVIDER_H
