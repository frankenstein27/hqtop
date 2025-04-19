#include "setting.h"

Setting::Setting() :
// 初始化 QSetting，指定组织名和应用名（影响存储路径,Linux下为 ～/.config/HeBo/hqtop.ini），设置格式为 INI 格式
    settings(QSettings::IniFormat, QSettings::UserScope, "HeBo", "hqtop")
{
    // 首次运行时 设置默认值
    if(!settings.contains("Window/Width"))
    {
        settings.setValue("Window/Width", 860);
        settings.setValue("Window/Height", 768);
        settings.setValue("Theme/Name", "Daytime");
        settings.setValue("Log/Level","debug");
    }
    // 立即写入磁盘
    settings.sync();
}

void Setting::save(const QString& key, const QVariant& value)
{
    settings.setValue(key, value);
//    qDebug() << "set new value successfully:" << key << " -> " << value;
}


Setting::~Setting()
{
    // 退出之前保存数据
    settings.sync();
}
