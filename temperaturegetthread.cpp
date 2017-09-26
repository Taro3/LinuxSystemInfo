#include <QThread>
#include <QFile>
#include <QDebug>

#include "temperaturegetthread.h"

//=====================================================================================================================
/**
 * @brief TemperatureGetThread::TemperatureGetThread
 * @param parent
 */
TemperatureGetThread::TemperatureGetThread(const int nCpuIndex)
    : QObject()
    , m_nCpuIndex(nCpuIndex)
{
}

//=====================================================================================================================
/**
 * @brief TemperatureGetThread::quit
 */
void TemperatureGetThread::quit()
{
    thread()->exit();
    thread()->wait();
}

//=====================================================================================================================
/**
 * @brief TemperatureGetThread::doWork
 */
void TemperatureGetThread::doWork()
{
    static const QString Temperature_FILE_PATH = "/sys/class/thermal/thermal_zone%1/temp";

    QString strFilename = Temperature_FILE_PATH.arg(m_nCpuIndex);
    QFile file(strFilename);

    if (!file.exists()) {
        qDebug() << strFilename + " is not found.";

        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << strFilename + " can't open.";

        return;
    }

    QString strLine = file.readLine();
    file.close();
    strLine = strLine.trimmed();
    quint64 nTemperature = strLine.toULongLong();
    emit finished(m_nCpuIndex, nTemperature);
}
