#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QSharedPointer>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>


class Logger
{
public:
    Logger();

    void shutdown_logger();


private:
    std::shared_ptr<spdlog::logger> spd_logger;

};

#endif // LOGGER_H
