#ifndef SETTING_H
#define SETTING_H

#include <QVariant>

class Setting
{
public:
    Setting();


    QVariant load(const QString& key);
    void save(const QString& key, const QVariant& value);


private:
    QString configPath;
};

#endif // SETTING_H
