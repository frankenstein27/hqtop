#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);




    Worker getCpuTotalTimeworker;
    QThread *getCpuTotalTimeThread = new QThread;

    getCpuTotalTimeworker.moveToThread(getCpuTotalTimeThread);
    connect(getCpuTotalTimeThread, &QThread::started,
                &getCpuTotalTimeworker, &Worker::work_getTotalCpuTime);
    getCpuTotalTimeThread->start();

    qDebug() << SystemResource::getCpuNum();

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
