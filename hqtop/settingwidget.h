#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QMessageBox>

#include <spdlog/spdlog.h>

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

    void on_DueDateComboBox_currentTextChanged(const QString &arg1);

signals:
    void logLevelChanged(QString newLevel);

private:
    Ui::SettingWidget *ui;
    Setting *setting;
    QString theme;
    QString logLevel;
    int m_interval_time;

    QString newTheme;
    QString newLogLevel;
    int m_newInterval_time;
    // 日志
    std::shared_ptr<spdlog::logger> mylogger;

};

#endif // SETTINGWIDGET_H
