#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

#include "setting.h"

namespace Ui {
class SettingWidget;
}

class SettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingWidget(Setting *setting, QWidget *parent = nullptr);
    ~SettingWidget();

private slots:
    // 日志级别发生变化
    void on_LogLevelComboBox_currentTextChanged(const QString &arg1);

    void on_applyPushButton_clicked();

    void on_cancelPushButton_clicked();

signals:
    void logLevelChanged(QString newLevel);

private:
    Ui::SettingWidget *ui;
    Setting *setting;
    QString theme;
    QString logLevel;

    QString newTheme;
    QString newLogLevel;
};

#endif // SETTINGWIDGET_H
