#include "settingwidget.h"
#include "ui_settingwidget.h"

SettingWidget::SettingWidget(Setting *setting, QWidget *parent) :
    QWidget(parent)
  , ui(new Ui::SettingWidget)
  , setting(setting)

{
    ui->setupUi(this);
    this->theme = this->setting->load<QString>("Theme/Name","Daytime");
    this->logLevel = this->setting->load<QString>("Log/Level","debug");


    if(this->theme == "Daytime")
        ui->DayModeRadioButton->setChecked(true);
    else if(this->theme == "Night")
        ui->NightModeRadioButton->setChecked(true);

    ui->LogLevelComboBox->setCurrentText(logLevel);

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
    else if(newTheme.size())
    {


        this->theme = newTheme;
        this->newTheme = "";
    }
    this->close();
}

void SettingWidget::on_cancelPushButton_clicked()
{
    if(this->newLogLevel.size() || this->newTheme.size())
    {
        QMessageBox::StandardButton result = QMessageBox::warning(nullptr, "exit",
                                                                  "settings has changed,want to save?",
                                                                  QMessageBox::No | QMessageBox::Yes);
        if(result == QMessageBox::Yes)
        {
            // 1.日志级别发生变化
            setting->save("Log/Level",this->newLogLevel);
            emit logLevelChanged(this->newLogLevel);
            this->logLevel = newLogLevel;
            this->newLogLevel = "";
            ui->applyPushButton->setEnabled(false);
            this->close();
        }
        else if(result == QMessageBox::No)
            this->close();
    }
    this->close();
}
