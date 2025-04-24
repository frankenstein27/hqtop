#include "logger.h"

#include <QDebug>
#include <QDir>

Logger::Logger(Setting *setting) :
    m_setting(setting)
{
    try
    {
        // 设置日志文件名、日志存放位置 同时设置以大小进行回滚 大小为 5MB，若写满20个文件将从头开始（覆盖第一个第二个第三个......）
        this->spd_logger =spdlog::rotating_logger_mt("global_logger", "./log/hqtop_log.txt",
                                                            1024 * 1024 * 5, 20);

        // 设置全局日志格式和日志级别
        this->spd_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

        this->currentLevel = m_setting->load<QString>("Log/Level", "warn");

        if(currentLevel == "trace")
        {
            this->spd_logger->set_level(spdlog::level::trace);
        }
        else if(currentLevel == "debug")
        {
            this->spd_logger->set_level(spdlog::level::debug);
        }
        else if(currentLevel == "info")
        {
            this->spd_logger->set_level(spdlog::level::info);
        }
        else if(currentLevel == "warn")
        {
            this->spd_logger->set_level(spdlog::level::warn);
        }
        else if(currentLevel == "err")
        {
            this->spd_logger->set_level(spdlog::level::err);
        }
        else if(currentLevel == "critical")
        {
            this->spd_logger->set_level(spdlog::level::critical);
        }
        else if(currentLevel == "off")
        {
            this->spd_logger->set_level(spdlog::level::off);
        }
        else
        {
            this->spd_logger->set_level(spdlog::level::warn);
        }
        // 配置自动刷新
        spdlog::flush_every(std::chrono::seconds(5));
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::string tmpStr = ex.what();
        this->spd_logger->warn("Log init failed: " + tmpStr);
        throw;
    }
}

// 日志等级更新
void Logger::logLevelChangedHandle(QString newLevel)
{
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


// 安全、完整的释放
void Logger::shutdown_logger()
{
    spdlog::drop("global_logger");  // 显式释放
    spdlog::shutdown();             // 确保所有资源释放
}
