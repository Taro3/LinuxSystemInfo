#include <QThread>

#include "cputemperature.h"
#include "osproc.h"
#include "temperaturegetthread.h"

//=====================================================================================================================
/**
 * @brief CpuTemperature::instance
 * @return
 */
CpuTemperature *CpuTemperature::instance()
{
    static CpuTemperature cpuTemperature;

    return &cpuTemperature;
}

//=====================================================================================================================
/**
 * @brief CpuTemperature::~CpuTemperature
 */
CpuTemperature::~CpuTemperature()
{
    for (QList<TemperatureGetThread*>::Iterator it = m_clstThreadl.begin(); it != m_clstThreadl.end(); ++it) {
        (*it)->quit();
        delete *it;
        *it = nullptr;
    }
}

//=====================================================================================================================
/**
 * @brief CpuTemperature::startGetCpuTemperature
 */
void CpuTemperature::startGetCpuTemperature()
{
    for (QList<TemperatureGetThread*>::Iterator it = m_clstThreadl.begin(); it != m_clstThreadl.end(); ++it) {
        QMetaObject::invokeMethod(*it, "doWork");
    }
}

//=====================================================================================================================
void CpuTemperature::getTemperatureFinished(const int nCpuIndex, const quint64 nTemperature)
{
    emit temperatureUpdated(nCpuIndex, nTemperature);
}

//=====================================================================================================================
/**
 * @brief CpuTemperature::CpuTemperature
 */
CpuTemperature::CpuTemperature() : QObject()
  , m_clstThreadl(QList<TemperatureGetThread*>())
{
    for (int i = 0; i < OsProc::instance()->cpuInfoCoreCount(); ++i) {
        TemperatureGetThread* pGetThread = new TemperatureGetThread(i);
        connect(pGetThread, SIGNAL(finished(int,quint64)), SLOT(getTemperatureFinished(int,quint64)));
        QThread *pWorkerThread = new QThread;
        pGetThread->moveToThread(pWorkerThread);
        pWorkerThread->start();
        m_clstThreadl.append(pGetThread);
    }
}
