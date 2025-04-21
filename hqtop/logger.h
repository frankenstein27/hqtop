#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QSharedPointer>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

#include <setting.h>


class Logger : public QObject
{
    Q_OBJECT

public:
    Logger(Setting *setting);

    void shutdown_logger();

public slots:
    void logLevelChangedHandle(QString newLevel);

private:
    std::shared_ptr<spdlog::logger> spd_logger;
    QString currentLevel;
    Setting *m_setting;
};

#endif // LOGGER_H
