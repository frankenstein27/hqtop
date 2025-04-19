#include "logger.h"

#include <QDebug>


Logger::Logger()
{
    try
    {
        // 设置日志文件名、日志存放位置 同时设置以大小进行回滚 大小为 5MB，若写满20个文件将从头开始（覆盖第一个第二个第三个......）
        this->spd_logger =spdlog::rotating_logger_mt("global_logger", "~/.config/HeBo", 1024 * 1024 * 5, 20);

        // 设置全局日志格式和日志级别
        this->spd_logger->set_pattern("[%n][%Y-%m-%d %H:%M:%S.%e] [%l] [%t]  %v");
        this->spd_logger->set_level(spdlog::level::warn);

        // 配置自动刷新
        spdlog::flush_every(std::chrono::seconds(5));
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        qDebug() << "Log init failed: " << ex.what();
        throw;
    }
}

void Logger::logLevelChangedHandle(QString newLevel)
{
//    qDebug() << "received new log level: " << newLevel;
    if(newLevel == "trace")
    {
        this->spd_logger->set_level(spdlog::level::trace);
    }
    else if(newLevel == "debug")
    {
        this->spd_logger->set_level(spdlog::level::debug);
    }
    else if(newLevel == "info")
    {
        this->spd_logger->set_level(spdlog::level::info);
    }
    else if(newLevel == "warn")
    {
        this->spd_logger->set_level(spdlog::level::warn);
    }
    else if(newLevel == "err")
    {
        this->spd_logger->set_level(spdlog::level::err);
    }
    else if(newLevel == "critical")
    {
        this->spd_logger->set_level(spdlog::level::critical);
    }
    else if(newLevel == "off")
    {
        this->spd_logger->set_level(spdlog::level::off);
    }
}


void Logger::shutdown_logger()
{
    spdlog::drop("global_logger");  // 显式释放
    spdlog::shutdown();             // 确保所有资源释放
}
