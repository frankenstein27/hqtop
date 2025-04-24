#include "settingwidget.h"
#include "ui_settingwidget.h"

SettingWidget::SettingWidget(Setting *setting, QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::SettingWidget)
  , setting(setting)
  , m_curThemeMode("")
  , m_newInterval_time(-1)
  , mylogger(spdlog::get("global_logger"))
{
    ui->setupUi(this);
    this->m_logLevel = this->setting->load<QString>("Log/Level","debug");
    this->m_interval_time = this->setting->load<int>("Timer(ms)/interval time",1000);
    this->m_themeMode = this->setting->load<QString>("Theme/Name","Daytime");
    this->m_curThemeMode = m_themeMode;
    if(m_themeMode == "Daytime")
    {
        ui->DayModeRadioButton->setChecked(true);
    }
    else if(m_themeMode == "Nighttime")
    {
        ui->NightModeRadioButton->setChecked(true);
    }
    else
    {
        mylogger->warn("wrong theme mode:don't exist,set default theme(Daytime)");
        ui->DayModeRadioButton->setChecked(true);
    }

    ui->LogLevelComboBox->setCurrentText(m_logLevel);
    ui->DueDateComboBox->setCurrentText(QString::number(m_interval_time));
    ui->applyPushButton->setEnabled(false);
}

SettingWidget::~SettingWidget()
{
    delete ui;
}

void SettingWidget::on_LogLevelComboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 != this->m_logLevel)
    {
        this->m_newLogLevel = arg1;
        ui->applyPushButton->setEnabled(true);
    }
    else
    {
        this->m_newLogLevel = "";
    }
}

void SettingWidget::on_DueDateComboBox_currentTextChanged(const QString &arg1)
{
    int interval_time = arg1.toInt();
    if(interval_time != this->m_interval_time)
    {
        this->m_newInterval_time = interval_time;
    }
    else
    {
        this->m_newInterval_time = -1;
    }
}

void SettingWidget::on_applyPushButton_clicked()
{
    if(m_newLogLevel.size())
    {
        // 处理日志级别发生变化
        setting->save("Log/Level",this->m_newLogLevel);
        emit logLevelChanged(this->m_newLogLevel);
        this->m_logLevel = m_newLogLevel;
        this->m_newLogLevel = "";
        ui->applyPushButton->setEnabled(false);
    }
    if(-1 != m_newInterval_time)
    {
        setting->save("Timer(ms)/interval time", m_newInterval_time);
        this->m_interval_time = m_newInterval_time;
        m_newInterval_time = -1;
        ui->applyPushButton->setEnabled(false);
    }
    if(this->m_themeMode != this->m_curThemeMode)
    {
        setting->save("Theme/Name", m_newThemeMode);
        this->m_themeMode = m_newThemeMode;
        this->m_curThemeMode = m_newThemeMode;
        this->m_newThemeMode = "";

        if(m_themeMode == "Daytime")
        {
            emit themeChanged("Daytime");
        }
        else if(m_themeMode == "Nighttime")
        {
            emit themeChanged("Nighttime");
        }

        ui->applyPushButton->setEnabled(false);
    }
    this->close();
}

void SettingWidget::on_cancelPushButton_clicked()
{
    if(this->m_newLogLevel.size() ||
            this->m_themeMode != this->m_curThemeMode ||
            -1 != m_newInterval_time
            )
    {
        QMessageBox::StandardButton result = QMessageBox::warning(nullptr, "exit",
                                                                  "settings has changed,want to save?",
                                                                  QMessageBox::No | QMessageBox::Yes);
        if(result == QMessageBox::Yes)
        {
            on_applyPushButton_clicked();
            this->close();
        }
        else if(result == QMessageBox::No)
            this->close();
    }
    this->close();
}

void SettingWidget::on_NightModeRadioButton_toggled(bool checked)
{
    if(!checked)
        m_themeMode = "Daytime";
    else
        m_themeMode = "Nighttime";

    if(!checked && m_themeMode != m_curThemeMode)
    {
        this->m_newThemeMode = "Daytime";
        ui->applyPushButton->setEnabled(true);
    }
    else if(checked && m_themeMode != m_curThemeMode)
    {
        this->m_newThemeMode = "Nighttime";
        ui->applyPushButton->setEnabled(true);
    }
}
