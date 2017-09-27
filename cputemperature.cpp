#include <QThread>
#include <QStringList>

#include "cputemperature.h"
#include "osproc.h"
#include "temperaturegetthread.h"
#include "sensorsdata.h"
#include "cpuinfodata.h"

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

    delete m_pcSendorsData;
    m_pcSendorsData = nullptr;
}

//=====================================================================================================================
/**
 * @brief CpuTemperature::startGetCpuTemperature
 */
void CpuTemperature::startGetCpuTemperature()
{
    // 温度データ読込 & 設定
    m_pcSendorsData->setSecsorsData(OsProc::instance()->createSensorsData());

    for (QList<TemperatureGetThread*>::Iterator it = m_clstThreadl.begin(); it != m_clstThreadl.end(); ++it) {
        QMetaObject::invokeMethod(*it, "doWork");
    }
}

//=====================================================================================================================
void CpuTemperature::getTemperatureFinished(const int nCpuIndex, const qreal nTemperature)
{
    emit temperatureUpdated(nCpuIndex, nTemperature);
}

//=====================================================================================================================
/**
 * @brief CpuTemperature::CpuTemperature
 */
CpuTemperature::CpuTemperature()
    : QObject()
    , m_pcSendorsData(nullptr)
    , m_clstThreadl(QList<TemperatureGetThread*>())
{
    // CPU温度データ保持オブジェクト生成
    m_pcSendorsData = new SensorsData(this);

    // CPU温度データ取得オブスレッド生成
    for (int i = 0; i < CpuinfoData::instance()->processorCount() + 1; ++i) {
        TemperatureGetThread* pGetThread = new TemperatureGetThread(m_pcSendorsData, i);
        connect(pGetThread, SIGNAL(finished(int,qreal)), SLOT(getTemperatureFinished(int,qreal)));
        QThread *pWorkerThread = new QThread;
        pGetThread->moveToThread(pWorkerThread);
        pWorkerThread->start();
        m_clstThreadl.append(pGetThread);
    }
}
