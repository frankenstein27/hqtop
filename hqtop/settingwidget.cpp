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
    this->m_CPUValue = this->setting->load<double>("Warning Value/CPU");
    this->m_newCPUValue = m_CPUValue;
    this->m_MemValue = this->setting->load<int>("Warning Value/Mem");
    this->m_newMemValue = m_MemValue;


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
    ui->CPUWariningComboBox->setCurrentText(QString::number(m_CPUValue) + "%");
    ui->MemWarningComboBox->setCurrentText(QString::number(m_MemValue) + "MB");
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
        ui->applyPushButton->setEnabled(true);
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
    if(this->m_CPUValue != this->m_newCPUValue)
    {
        this->setting->save("Warning Value/CPU", m_newCPUValue);
        emit CPUWarningValueChanged(m_newCPUValue);
        this->m_CPUValue = this->m_newCPUValue;
        ui->applyPushButton->setEnabled(false);
    }
    if(this->m_MemValue != this->m_newMemValue)
    {
        this->setting->save("Warning Value/Mem", m_newMemValue);
        emit MemWarningValueChanged(m_newMemValue);
        this->m_MemValue = this->m_newMemValue;
        ui->applyPushButton->setEnabled(false);
    }

    this->close();
}

void SettingWidget::on_cancelPushButton_clicked()
{
    if(this->m_newLogLevel.size() ||
            this->m_themeMode != this->m_curThemeMode ||
            -1 != m_newInterval_time ||
            this->m_CPUValue != this->m_newCPUValue ||
            this->m_MemValue != this->m_newMemValue
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

void SettingWidget::on_CPUWariningComboBox_currentTextChanged(const QString &arg1)
{
    int startIndex = arg1.indexOf("%");
    QString res = arg1;
    res.remove(startIndex,1);
    this->m_newCPUValue = res.toDouble();

    if(this->m_newCPUValue != m_CPUValue)
        ui->applyPushButton->setEnabled(true);
    else
        ui->applyPushButton->setEnabled(false);
}

void SettingWidget::on_MemWarningComboBox_currentTextChanged(const QString &arg1)
{
    int startIndex = arg1.indexOf("MB");
    QString res = arg1;
    res.remove(startIndex,2);
    this->m_newMemValue = res.toInt();

    if(m_newMemValue != m_MemValue)
        ui->applyPushButton->setEnabled(true);
    else
        ui->applyPushButton->setEnabled(false);
}
