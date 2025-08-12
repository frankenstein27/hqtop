# Hqtop
### 1.项目定位
基于Qt框架开发的一款低资源占用的、跨平台（Windows/Linux）的系统资源监控工具，支持实时追踪进程状态、CPU/内存使用率及系统运行信息。

### 2.技术栈与关键功能

使用Qt 5.14 + QML 实现交互式UI，使用 QCharts 实时展示进程信息，以及 QViews 实时绘制系统资源图表，利用多线程（QThread）后台处理数据提升程序流畅性。
### 3.项目模块与架构
项目采用分层架构，由顶至下分别为：GUI界面层，业务逻辑层，数据采集层。各层工作内容独立，逻辑紧密但耦合度低。采用了组合模式、工厂模式、单例模式、模版方法模式等设计模式。

------
### 1. Project positioning
Based on Qt framework, a low resource consumption, cross-platform (Windows/Linux) system resource monitoring tool is developed, which supports real-time tracking of process status, CPU/ memory usage and system operation information.
### 2. Stack and key features
Qt 5.14 + QML is used to realize interactive UI, QCharts is used to display process information in real time, and QViews is used to draw system resource charts in real time. QThread is used to process data in the background to improve program fluency.
### 3. Project modules and architecture
The project adopts hierarchical architecture, which is divided into GUI interface layer, business logic layer and data collection layer from top to bottom. Each layer does its own job, closely related but not highly coupled. Design patterns such as composition pattern, factory pattern, singleton pattern and template method pattern are adopted.


windows 平台
------
运行效果
<img width="1609" height="1440" alt="image" src="https://github.com/user-attachments/assets/20b5407f-02fd-4cc0-89ec-d8ef1ef74b01" />

筛选效果1
<img width="1720" height="1314" alt="image" src="https://github.com/user-attachments/assets/a40de795-e76b-4d3f-8bc8-c9d86002196b" />

筛选效果2
<img width="1642" height="1302" alt="image" src="https://github.com/user-attachments/assets/04b7a995-7228-45f7-8d1f-7863cba9ee0b" />

根据cpu利用率排序
<img width="1642" height="1302" alt="image" src="https://github.com/user-attachments/assets/eeb3a662-b786-4e8d-8cbb-3a1b432e1b41" />

根据内存使用量排序
<img width="1642" height="1302" alt="image" src="https://github.com/user-attachments/assets/2e3e1f18-7d68-4268-acbf-9cddaea0c5a8" />

资源总览页面
<img width="1628" height="1316" alt="image" src="https://github.com/user-attachments/assets/5d60ffa2-b5e3-4727-a0db-0f6c814505dc" />

设置界面
<img width="1298" height="792" alt="image" src="https://github.com/user-attachments/assets/3e167f38-8044-4495-be11-4a5ebb4f888e" />

夜间模式效果
<img width="1642" height="1302" alt="image" src="https://github.com/user-attachments/assets/b9c91efd-7698-49b3-be8f-447d274247ef" />

进程占用过高警告效果
<img width="1642" height="1302" alt="image" src="https://github.com/user-attachments/assets/c63f7f6b-6e80-4bb7-bb35-887a9a9f834e" />

杀死进程展示
<img width="1642" height="1302" alt="image" src="https://github.com/user-attachments/assets/24a609cb-a4b9-44f1-9efa-0b7f3712c273" />
<img width="1676" height="1297" alt="image" src="https://github.com/user-attachments/assets/16447411-ed76-4c9c-82eb-e5a066f1301c" />

日志展示
<img width="1232" height="326" alt="image" src="https://github.com/user-attachments/assets/1d37b566-919e-497f-a997-3b94fdf8a5dd" />

运行时占用系统资源量极少
<img width="1461" height="515" alt="image" src="https://github.com/user-attachments/assets/bf53e05e-d059-4056-a058-c104fdfabd3e" />

与系统任务管理器对比效果
<img width="1592" height="944" alt="image" src="https://github.com/user-attachments/assets/71dd11b8-8b23-4211-9e29-58793088867c" />


------
Linux平台
运行效果
<img width="919" height="791" alt="image" src="https://github.com/user-attachments/assets/4f36f54b-d125-456a-af5f-a8fec8aa22f1" />

筛选效果1
<img width="962" height="803" alt="image" src="https://github.com/user-attachments/assets/4fca3029-0b4a-40a5-a607-95ba362487f6" />

筛选效果2
<img width="926" height="810" alt="image" src="https://github.com/user-attachments/assets/752d1727-bb7d-4a5e-b52e-4d808e729932" />

筛选效果3
<img width="891" height="798" alt="image" src="https://github.com/user-attachments/assets/fa0fad29-0058-40a5-b4dc-92f4684f6671" />

筛选效果4
<img width="868" height="835" alt="image" src="https://github.com/user-attachments/assets/ea6dbb0b-54c3-43da-a701-cc5d0555b097" />

根据cpu利用率排序
<img width="908" height="810" alt="image" src="https://github.com/user-attachments/assets/d4a0f6a7-3551-4cdd-beb4-3d5adab6e2d3" />

根据内存使用量排序
<img width="882" height="786" alt="image" src="https://github.com/user-attachments/assets/a70f29fd-f92b-421b-aee7-6c455dc4133a" />

资源总览界面
<img width="857" height="697" alt="image" src="https://github.com/user-attachments/assets/a03ec37f-0767-4ec1-9a8d-df8eac6d9515" />

设置界面
<img width="670" height="430" alt="image" src="https://github.com/user-attachments/assets/79564f99-a300-4c34-acc1-28a2cc95bf88" />

夜间模式效果
<img width="853" height="774" alt="image" src="https://github.com/user-attachments/assets/a3a07c1e-6898-40d9-90eb-619c4c287799" />

进程占用过高警告
<img width="857" height="777" alt="image" src="https://github.com/user-attachments/assets/847f2529-989d-4eb1-9a25-11bcd3c96b4c" />

选中进程并杀死
<img width="854" height="785" alt="image" src="https://github.com/user-attachments/assets/ef4c6efa-3400-4ad0-ae14-0b47782a2902" />
<img width="430" height="156" alt="image" src="https://github.com/user-attachments/assets/1174e00b-92c5-4355-b6a9-8ad0fd0a7534" />
<img width="404" height="92" alt="image" src="https://github.com/user-attachments/assets/42a10010-187f-4352-bacb-c4ffcfc4a307" />

日志展示
<img width="1525" height="275" alt="image" src="https://github.com/user-attachments/assets/758732f5-c536-47f0-8d89-281009cb24e6" />

运行时系统资源占用极少
<img width="850" height="434" alt="image" src="https://github.com/user-attachments/assets/a6a83bc2-ae50-4b7f-9584-19178758ce6e" />

与htop开源工具对比效果，差距较小
<img width="712" height="431" alt="image" src="https://github.com/user-attachments/assets/8f44fb80-28b2-4b39-be0a-45d09ede35c9" />
