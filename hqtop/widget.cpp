#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    SystemDataProvider *m_linuxDataProvider = new LinuxDataProvider;



}

Widget::~Widget()
{
    delete ui;
}

void Widget::updateProcessList()
{

}
