#ifndef SETTING_H
#define SETTING_H

#include <QVariant>
#include <QSettings>
#include <QDebug>
#include <QFile>

#include <spdlog/spdlog.h>

class Setting
{
public:
    Setting();
    ~Setting();

    // 加载配置，模板函数，支持类型推断
    template<typename T>
    const T load(const QString& key, const T& defaultValue = T())
    {
        return settings.value(key, defaultValue).template value<T>();
    }

    void save(const QString& key, const QVariant& value);


private:
    QSettings settings;
 };

#endif // SETTING_H
