# hqtop
### 1.项目定位
基于Qt框架开发的一款低资源占用的、跨平台（Windows/Linux）的系统资源监控工具，支持实时追踪进程状态、CPU/内存使用率及系统运行信息。
### 2.技术栈与关键功能
使用Qt 5.14 + QML 实现交互式UI，使用 QCharts 实时展示进程信息，以及 QViews 实时绘制系统资源图表，利用多线程（QThread）后台处理数据提升程序流畅性。
### 3.项目模块与架构
项目采用分层架构，由顶至下分别为：GUI界面层，业务逻辑层，数据采集层。每层各司其职，关联紧密但耦合度不高。采用了组合模式、工厂模式、单例模式、模版方法模式等设计模式。

------
### 1. Project positioning
Based on Qt framework, a low resource consumption, cross-platform (Windows/Linux) system resource monitoring tool is developed, which supports real-time tracking of process status, CPU/ memory usage and system operation information.
### 2. Stack and key features
Qt 5.14 + QML is used to realize interactive UI, QCharts is used to display process information in real time, and QViews is used to draw system resource charts in real time. QThread is used to process data in the background to improve program fluency.
### 3. Project modules and architecture
The project adopts hierarchical architecture, which is divided into GUI interface layer, business logic layer and data collection layer from top to bottom. Each layer does its own job, closely related but not highly coupled. Design patterns such as composition pattern, factory pattern, singleton pattern and template method pattern are adopted.
