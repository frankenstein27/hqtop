#include "resourcewidget.h"
#include "ui_resourcewidget.h"

ResourceWidget::ResourceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ResourceWidget)
{
    ui->setupUi(this);
}

ResourceWidget::~ResourceWidget()
{
    delete ui;
}
