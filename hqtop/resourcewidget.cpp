#include "resourcewidget.h"
#include "ui_resourcewidget.h"

 /*
  *
  */

ResourceWidget::ResourceWidget(ResourceAnalyzer *resourceanalyzer,QWidget *parent) :
    QWidget(parent),
    m_resourceanalyzer(resourceanalyzer),
    ui(new Ui::ResourceWidget)
{
    ui->setupUi(this);

    connect(this->m_resourceanalyzer, &ResourceAnalyzer::systemResourceUpdate,
                    this, &ResourceWidget::onSystsemReourceUpdate);
}

void ResourceWidget::onSystsemReourceUpdate(SystemResource newSystemResource)
{
    this->m_sysResource = newSystemResource;
    emit systemResourceUpdate(m_sysResource);
}

ResourceWidget::~ResourceWidget()
{
    delete ui;
    delete this->m_resourceanalyzer;
}
