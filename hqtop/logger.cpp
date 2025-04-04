#include "logger.h"

#include <QDebug>


Logger::Logger()
{
    try
    {
        // 设置日志文件名 同时设置以大小进行回滚
        this->spd_logger =spdlog::rotating_logger_mt("global_logger", "./log/hqtop.log", 10240, 20);

        // 设置全局日志格式和日志级别
        this->spd_logger->set_pattern("[%n][%Y-%m-%d %H:%M:%S.%e] [%l] [%t]  %v");
        this->spd_logger->set_level(spdlog::level::debug);

        // 配置自动刷新
        spdlog::flush_every(std::chrono::seconds(5));
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        qDebug() << "Log init failed: " << ex.what();
        throw;
    }
}

void Logger::shutdown_logger()
{
    spdlog::drop("global_logger");  // 显式释放
    spdlog::shutdown();             // 确保所有资源释放
}
