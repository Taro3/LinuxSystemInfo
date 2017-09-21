#include "loadgetthread.h"
#include "loaddata.h"

LoadGetThread::LoadGetThread(LoadData * const pLoadData, const int nCpuId, QObject *parent) : QObject(parent)
  , m_nCpuId(0)
  , m_pcLoadData(nullptr)
{
    m_nCpuId = nCpuId;
    m_pcLoadData = pLoadData;
}

void LoadGetThread::doWork()
{
    LoadData ld;
    ld.lock();
    QStringList lstLoad = ld.loadData();
    ld.unlock();

    qreal nLoad = 0;
    /** @todo 負荷率取得処理実装 */
    emit finished(m_nCpuId, nLoad);
}
