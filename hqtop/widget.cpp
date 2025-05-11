#include "widget.h"
#include "ui_widget.h"


/* 本类涉及到的多为与 UI 有关操作，因此无法在子线程中运行，封装的底层函数直接调用即可完成程序功能
 *
 */

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 启用 tableView 排序功能
    ui->processesTableView->setSortingEnabled(true);
    ui->processesTableView->setAlternatingRowColors(true);
    ui->processesTableView->setStyleSheet("");

    // 初始化设置
    this->setting = new Setting();
    // 初始化日志
    this->logger = new Logger(setting);
    this->mylogger = spdlog::get("global_logger");

    // 获取设置中的窗口等默认配置
    int windowWidth = setting->load("Window/Width", 860);
    QString theme = setting->load<QString>("Theme/Name", "Daytime");
    int windowHeight = setting->load<int>("Window/Height", 720);

    QPalette mainPal(this->palette());

    loadTheme(theme);
    this->setAutoFillBackground(true);
    this->setPalette(mainPal);


    this->setFixedSize(windowWidth, windowHeight);

#ifdef Q_OS_WIN

    this->dataprovider = new WindowsDataProvider();

#elif defined(Q_OS_LINUX)

    this->dataprovider = new LinuxDataProvider();

#endif
    this->dataCollector = new DataCollector(this->dataprovider);
    this->processmanager = new ProcessesManager();
    this->resourceanalyzer = new ResourceAnalyzer();
    this->myTableModel = new ProcessTableModel(this->setting, this->processmanager, ui->processesTableView);
    this->resourceWidget = new ResourceWidget(this->resourceanalyzer);
    this->settingWidget = new SettingWidget(setting);


    // 启动程序即开始收集数据

    /* 处理进程更新
     * 1.接收 dataCollector 传来的信号 updateProcesses 进程信息更新 指定由 processManager 中的函数处理
     * 由于 dataCollector 整个对象都被移至子线程中，所以此信号也由子线程发出
     */
    connect(this->dataCollector, &DataCollector::updateProcesses, this->processmanager,
                    &ProcessesManager::handldProcessUpdate);

    /* 2.收集进程和系统资源
     * 若切换到进程页面（默认为此页面），收集进程和系统资源信息
     * 2.1.接收dataCollector 的信号:updateSysResourceToProcessesPage进程页系统资源更新,由ResourceAnalyzer中的函数处理
     */
    connect(this->dataCollector, &DataCollector::updateSysResourceToProcessesPage, this->resourceanalyzer,
                    &ResourceAnalyzer::handleSystemResourceUpdate);

    /* 若切换到系统资源页面，只收集系统资源信息
     * 2.2.接收dataCollector 的信号:updateSysResourceToSysResourcePage 系统页系统资源信息更新,由ResourceWidget中的函数处理
     */
    connect(this->dataCollector, &DataCollector::updateSysResourceToSysResourcePage, this->resourceWidget,
                    &ResourceWidget::handleSysResourceUpdateSysResPage);

    /* 3.将 resourceWidget 收到的数据显示到 ui 中
     * 接受 ResourceWidget 传来的信号 systemResourceUpdate 系统资源更新
     * 指定由本类的 onSystemResourceUpdate 函数处理
     */
    connect(this->resourceWidget, &ResourceWidget::systemResourceUpdate, this,
                    &Widget::onSystemResourceUpdate);

    /* 4.筛选
     * 4.1.筛选：传递筛选因子给 processTableModel
     */
    connect(ui->FilterComboBox, &QComboBox::currentTextChanged,
                    this->myTableModel, &ProcessTableModel::filterComboBoxChanged);

    /* 4.2.筛选：将筛选框中的内容传递给 processTableModel，筛选完成刷新数据再显示
     */
    connect(ui->FilterLineEdit, &QLineEdit::textChanged,
                    this->myTableModel, &ProcessTableModel::filterLineEditChanged);

    /* 5.进程数量统计
     * 将进程数量更新信号 processesNumberChanged 与 onProcessesNumberChanged 关联
     */
    connect(this->myTableModel, &ProcessTableModel::processesNumberChanged,
                    this, &Widget::onProcessesNumberChanged);

    /* 6.杀死进程
     * 6.1.将选中的进程传递给 processTableModel (!!! QTableView操作 !!!)
     */
    connect(ui->processesTableView, &QTableView::clicked,
                    this->myTableModel, &ProcessTableModel::onTableRowClicked);

    /* 6.2.删除按钮被按下 由 ProcessTableModel 的 onDeletePushButtonClicked 接收
     */
    connect(ui->deletePushButton, &QPushButton::clicked,
                    this->myTableModel, &ProcessTableModel::onDeletePushButtonClicked);

    /* 6.3.processmanager 的杀死进程信号发出 由 LinuxDataProvider的killProcess 接收
     */
//#ifdef Q_OS_WIN
//    connect(this->processmanager, &ProcessesManager::killProcessSignal,
//                    this->dataprovider, &WindowsDataProvider::killProcess);
//#elif defined (Q_OS_LINUX)
//    connect(this->processmanager, &ProcessesManager::killProcessSignal,
//                    this->dataprovider, &LinuxDataProvider::killProcess);
//#endif
    connect(this->processmanager, &ProcessesManager::killProcessSignal,
                    this->dataprovider, &SystemDataProvider::killProcess);


    /* 7.进程页和资源页相互切换，同时控制相关底层函数调用，节约性能
     * 7.1.在进程页点击资源页按钮 发出 processesPageHide 信号（取消调用进程更新函数，但不取消调用系统更新函数）
     */
    connect(this, &Widget::processesPageHide, this->dataCollector, &DataCollector::onProcessesPageHide);

    /* 7.2.在资源页点击进程页按钮 由 ResourceWidget 的进程页按钮发出信号：processesPageShow
     * DataCollector 中的 onProcessesPageShow 接收（控制流程调用 进程更新函数 和 系统更新函数 ）
     * 通知收集者收集进程信息
     */
    connect(this->resourceWidget, &ResourceWidget::processesPageShow,
                    this->dataCollector, &DataCollector::onProcessesPageShow);

    /* 7.3.将进程页重新设置为可见
     * 返回进程页信号 由 ResourceWidget 的进程页按钮发出本类中的 onProcessesPageShow 接收
     */
    connect(this->resourceWidget, &ResourceWidget::processesPageShow,
                    this, &Widget::onProcessesPageShow);

    /* 8.设置中日志等级改变
     *  日志等级改变信号 由 settingWidget 的 logLevelChanged 发出，Logger的 logLevelChangedHandle 接收
     */
    connect(this->settingWidget, &SettingWidget::logLevelChanged,
                    this->logger, &Logger::logLevelChangedHandle);


    /*
     *
     */
    connect(this->settingWidget, &SettingWidget::themeChanged,
                    this, &Widget::loadTheme);

    int interval_time = setting->load<int>("Timer(ms)/interval time");

    // 启动收集者 指定时间间隔为 interval_time
    dataCollector->startCollection(interval_time);

    // 将 TableView 的内容显示出来
    ui->processesTableView->setModel(this->myTableModel);
}

void Widget::onSystemResourceUpdate(SystemResource newSystemResource)
{
    ui->cpuLabel->setText(QString::number(newSystemResource.getCpuTotal()) + "%");
    ui->upTimeLabel->setText(newSystemResource.getUpTime());
    ui->memUsedLabel->setText(QString::number(newSystemResource.getMemoryUsed()) + "GB");
    ui->memTotalLabel->setText(QString::number(newSystemResource.getMemoryTotal()) + "GB");
    ui->swapUsedLabel->setText(QString::number(newSystemResource.getSwapUsed()) + "MB");
    ui->swapTotalLabel->setText(QString::number(newSystemResource.getSwapTotal()) + "GB");
}

void Widget::onProcessesNumberChanged(qint64 processesNumber)
{
    ui->processesNumberLabel->setText(QString::number(processesNumber));
}

Widget::~Widget()
{
    delete ui;
    delete resourceWidget;
    delete myTableModel;
    delete resourceanalyzer;
    delete processmanager;
    delete dataCollector;
    delete dataprovider;
    logger->shutdown_logger();
    delete logger;
}

void Widget::onProcessesPageShow()
{
    if(!this->isVisible())
    {
        this->show();
        emit this->processesPageShow();
    }
}


void Widget::on_resourcePagePushButton_clicked()
{
    // 进程页设置为不可见 发出信号通知收集者采取对应措施，不要收集不显示的进程信息
    if(this->isVisible())
    {
        this->hide();
        emit this->processesPageHide();
    }
    resourceWidget->show();
}


void Widget::on_settingsButton_clicked()
{
    // 设置子窗口模态：子窗口存在时不能操作父窗口
    this->settingWidget->setWindowModality(Qt::ApplicationModal);
    this->settingWidget->show();
}

void Widget::loadTheme(QString newTheme)
{
    QString path;
    if(newTheme == "Daytime")
    {
        path = ":/styles/day.qss";
    }
    else if(newTheme == "Nighttime")
    {
        path = ":/styles/night.qss";
    }
    QFile qssFile(path);
    if(qssFile.open(QIODevice::ReadOnly))
    {
        mylogger->trace("read qss file:" + path.toStdString() + " successfully.");
        qApp->setStyleSheet(qssFile.readAll());
        qssFile.close();
    }
}
