#include <QDebug>
#include <QThread>

#include "loadgetthread.h"
#include "loaddata.h"

//=====================================================================================================================
/**
 * @brief       LoadGetThread::LoadGetThread
 *              コンストラクタ
 * @param[in]   pLoadData   負荷情報テキスト保持オブジェクトポインタ
 * @param[in]   nCpuId      CPU-ID
 * @arg         0           CPU全体
 * @arg         1-          論理CPU番号 + 1
 * @param[in]   parent      親オブジェクトポインタ(QThreadで別スレッド動作させるため nullptr を指定すること
 * @return      なし
 */
LoadGetThread::LoadGetThread(LoadData * const pLoadData, const int nCpuId, QObject *parent) : QObject(parent)
  , m_nCpuId(0)
  , m_pcLoadData(nullptr)
{
    m_nCpuId = nCpuId;
    m_pcLoadData = pLoadData;
}

//=====================================================================================================================
/**
 * @brief   LoadGetThread::doWork
 *          負荷率を取得し完了したらシグナルを発行する
 * @return  なし
 */
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
