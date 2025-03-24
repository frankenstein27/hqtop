#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    SystemDataProvider *m_LinuxDataProvider = new LinuxDataProvider;
    m_LinuxDataProvider->getProcessList();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateProcessList()
{

}

void Widget::on_testPushButton_clicked()
{

}
