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

    void on_NightModeRadioButton_toggled(bool checked);

    void on_MemWarningComboBox_currentTextChanged(const QString &arg1);

    void on_CPUWariningComboBox_currentTextChanged(const QString &arg1);

signals:
    void logLevelChanged(QString newLevel);
    void themeChanged(QString newTheme);
    void CPUWarningValueChanged(double newWarningValue);
    void MemWarningValueChanged(int newWarningValue);

private:
    Ui::SettingWidget *ui;
    Setting *setting;
    QString m_themeMode;
    QString m_logLevel;
    int m_interval_time;
    QString m_curThemeMode;
    double m_CPUValue;
    int m_MemValue;

    QString m_newLogLevel;
    QString m_newThemeMode;
    double m_newCPUValue;
    int m_newMemValue;

    int m_newInterval_time;
    // 日志
    std::shared_ptr<spdlog::logger> mylogger;

};

#endif // SETTINGWIDGET_H
