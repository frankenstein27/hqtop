#include "settingwidget.h"
#include "ui_settingwidget.h"

SettingWidget::SettingWidget(Setting *setting, QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::SettingWidget)
  , setting(setting)
  , m_newInterval_time(-1)
  , mylogger(spdlog::get("global_logger"))

{
    ui->setupUi(this);
    this->theme = this->setting->load<QString>("Theme/Name","Daytime");
    this->logLevel = this->setting->load<QString>("Log/Level","debug");
    this->m_interval_time = this->setting->load<int>("Timer(ms)/interval time",1000);

    if(this->theme == "Daytime")
        ui->DayModeRadioButton->setChecked(true);
    else if(this->theme == "Night")
        ui->NightModeRadioButton->setChecked(true);

    ui->LogLevelComboBox->setCurrentText(logLevel);
    ui->DueDateComboBox->setCurrentText(QString::number(m_interval_time));
    ui->applyPushButton->setEnabled(false);
}

SettingWidget::~SettingWidget()
{
    delete ui;
}

void SettingWidget::on_LogLevelComboBox_currentTextChanged(const QString &arg1)
{
    if(arg1 != this->logLevel)
    {
        this->newLogLevel = arg1;
        ui->applyPushButton->setEnabled(true);
    }
    else
    {
        this->newLogLevel = "";
        ui->applyPushButton->setEnabled(false);
    }
}

void SettingWidget::on_DueDateComboBox_currentTextChanged(const QString &arg1)
{
    int interval_time = arg1.toInt();
    if(interval_time != this->m_interval_time)
    {
        this->m_newInterval_time = interval_time;
        ui->applyPushButton->setEnabled(true);
    }
    else
    {
        this->m_newInterval_time = -1;
        ui->applyPushButton->setEnabled(false);
    }
}

void SettingWidget::on_applyPushButton_clicked()
{
    if(newLogLevel.size())
    {
        // 处理日志级别发生变化
        setting->save("Log/Level",this->newLogLevel);
        emit logLevelChanged(this->newLogLevel);
        this->logLevel = newLogLevel;
        this->newLogLevel = "";
        ui->applyPushButton->setEnabled(false);
    }
    if(newTheme.size())
    {


        this->theme = newTheme;
        this->newTheme = "";
        ui->applyPushButton->setEnabled(false);
    }
    if(-1 != m_newInterval_time)
    {
        setting->save("Timer(ms)/interval time", m_newInterval_time);
        this->m_interval_time = m_newInterval_time;
        m_newInterval_time = -1;
        ui->applyPushButton->setEnabled(false);
    }
    this->close();
}

void SettingWidget::on_cancelPushButton_clicked()
{
    if(this->newLogLevel.size() || this->newTheme.size() || -1 != m_newInterval_time)
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
