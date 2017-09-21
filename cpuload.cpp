#include <QDebug>
#include <sys/times.h>
#include <QThread>

#include "cpuload.h"
#include "osproc.h"
#include "loaddata.h"
#include "loadgetthread.h"

//=====================================================================================================================
/**
 * @brief       CpuLoad::CpuLoad
 *              コンストラクタ
 * @param[in]   parent  親オブジェクトポインタ
 * @return      なし
 */
CpuLoad::CpuLoad(QObject *parent) : QObject(parent)
  , m_pcOsProc(nullptr)
  , m_pcLoadData(nullptr)
  , m_nProcessorCount(0)
  , m_isReady(false)
{
}

CpuLoad::~CpuLoad()
{
    for (int i = 0; i < m_lstGetThread.count(); ++i) {
        m_lstGetThread.at(i)->quit();
        delete m_lstGetThread.at(i);
        m_lstGetThread.replace(i, nullptr);
    }
    delete m_pcLoadData;
    m_pcLoadData = nullptr;
    delete m_pcOsProc;
    m_pcOsProc = nullptr;
}

//=====================================================================================================================
/**
 * @brief   CpuLoad::initialize
 *          初期化
 * @return  初期化結果
 * @retval  true    正常終了
 * @retval  false   エラー
 */
bool CpuLoad::initialize()
{
    if (m_isReady) {
        return true;
    }

    m_pcLoadData = new LoadData(this);
    m_pcOsProc = new OsProc(this);
    m_nProcessorCount = m_pcOsProc->cpuInfoProcessorCount();

    if (m_nProcessorCount == 0) {
        qDebug() << QString(__func__) + " (" + __LINE__ + "): can't get cpu count.";

        return false;
    }

    for (int i = 0; i < m_nProcessorCount + 1; ++i) {
        LoadGetThread *pGetThread = new LoadGetThread(m_pcLoadData, i);
        m_lstGetThread.append(pGetThread);
        connect(pGetThread, SIGNAL(finished(int,qreal)), SLOT(getLoadFinished(int,qreal)));
        QThread *pThread = new QThread(this);
        pGetThread->moveToThread(pThread);
        pThread->start();
//        m_lstSubThread.append(pThread);
    }

#if 0
    QList<QList<quint64> > initLoads = m_pcOsProc->statCpus();
    clock_t now = ::times(nullptr);

    for (int i = 0; i < m_nProcessorCount + 1; ++i) {
        sInfoAndTime sIat;
        sIat.lstInfo = initLoads[i];
        sIat.prevClock = now;
        m_vecCpuInfo.append(sIat);
    }
#endif

    m_isReady = true;

    return true;
}

//=====================================================================================================================
/**
 * @brief   CpuLoad::cpuLoad
 *          CPU処理時間取得
 * @return  処理時間(全体1 +  論理CPU数)
 * @note    QMLから呼び出し可能
 */
QList<qreal> CpuLoad::cpuLoad()
{
    QList<qreal> allLoads;

    if (!m_isReady) {
        qDebug() << QString(__func__) + " (" + __LINE__ + "): invalid parameter.";
        return allLoads;
    }

    QList<QList<quint64> > currentStats = m_pcOsProc->statCpus();
    clock_t now = ::times(nullptr);

    for (int i = 0; i < currentStats.count(); ++i) {
        quint64 currentLoad = currentStats[i][0] + currentStats[i][1] + currentStats[i][2];
        quint64 prevLoad
                = m_vecCpuInfo[i].lstInfo[0] + m_vecCpuInfo[i].lstInfo[1] + m_vecCpuInfo[i].lstInfo[2];
        qreal load = static_cast<qreal>(currentLoad - prevLoad) / (now - m_vecCpuInfo[i].prevClock) * 100.0f;

        if (i == 0) {
            load /= m_nProcessorCount;
        }

        allLoads << load;
        sInfoAndTime sIat;
        sIat.lstInfo = currentStats[i];
        sIat.prevClock = now;
        m_vecCpuInfo.replace(i, sIat);
    }

    return allLoads;
}

void CpuLoad::startGetCpuLoad()
{
    QStringList lstLoadFile = m_pcOsProc->loadStatFile();
    m_pcLoadData->setLoadData(lstLoadFile);
    for (int i = 0; i < m_lstGetThread.count(); ++i) {
        if (QMetaObject::invokeMethod(m_lstGetThread.at(i), "doWork") == false) {
            qDebug() << "invokeMethod failed.";
        }
    }
}

void CpuLoad::getLoadFinished(const int nCpuIndex, const qreal load)
{
    emit loadUpdated(nCpuIndex, load);
}
