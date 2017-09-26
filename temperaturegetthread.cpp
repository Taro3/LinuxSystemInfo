#include <QThread>
#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QTextStream>

#include "temperaturegetthread.h"
#include "sensorsdata.h"
#include "osproc.h"

//=====================================================================================================================
/**
 * @brief TemperatureGetThread::TemperatureGetThread
 * @param parent
 */
TemperatureGetThread::TemperatureGetThread(SensorsData *pSensorsData, const int nCpuIndex)
    : QObject()
    , m_nCpuIndex(nCpuIndex)
    , m_pcSensorsData(pSensorsData)
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
    const static QString STR_PACKAGE_ID = "^Package\\sid\\s0:.*";
    const static QString STR_CORE       = "^Core\\s%1:.*";

    QStringList lstData = m_pcSensorsData->sensorsData();

    // Package0を検索
    int nPackageIndex = lstData.indexOf(QRegExp(STR_PACKAGE_ID, Qt::CaseInsensitive));
    if (nPackageIndex < 0) {
        qDebug() << "Package line not found.";

        return;
    }
    qreal nTemperature = 0.0f;
    if (m_nCpuIndex == 0) {
        // CPU全体
        QString strLine = lstData.at(nPackageIndex);
        QStringList lstLine = strLine.split(':');
        if (lstLine.count() < 2) {
            qDebug() << "Invalid 'Package' line.";

            return;
        }
        QString strTemp = lstLine.at(1);
//        strTemp = strTemp.trimmed();
        QTextStream stream(&strTemp);
        stream >> nTemperature;
    } else {
        // 各Core
        int nIndex = nPackageIndex;
        for (int i = 0; i < OsProc::instance()->cpuInfoCoreCount(); ++i) {
            nIndex = lstData.indexOf(QRegExp(STR_CORE.arg(i), Qt::CaseInsensitive));
            if (nIndex < 0) {
                qDebug() << QString("Core ") + i + "data not found.";

                return;
            }
            QString strLine = lstData.at(nIndex);
            QStringList lstLine = strLine.split(':');
            if (lstLine.count() < 2) {
                qDebug() << "Invalid 'Core' line.";

                return;
            }
            QString strTemp = lstLine.at(1);
//            strTemp = strTemp.trimmed();
            QTextStream stream(&strTemp);
            stream >> nTemperature;
        }
    }

    emit finished(m_nCpuIndex, nTemperature);
}
