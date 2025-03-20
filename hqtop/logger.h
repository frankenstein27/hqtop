#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QSharedPointer>
#include "spdlog/spdlog.h"


class Logger
{
public:
    Logger();

    static void logInfo(const QString& message);
    static void logError(const QString& message);

private:
    static std::shared_ptr<spdlog::logger> spd_logger;

};

#endif // LOGGER_H
