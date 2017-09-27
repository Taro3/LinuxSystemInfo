#include <QDebug>
#include <QThread>
#include <QStringList>

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
LoadGetThread::LoadGetThread(LoadData * const pLoadData, const int nCpuId, const int nTotalCpuCount, QObject *parent)
    : QObject(parent)
    , m_nCpuId(0)
    , m_nTotalCpuCount(0)
    , m_pcLoadData(nullptr)
    , m_nPrevLoad(0)
{
    m_nCpuId = nCpuId;
    m_nTotalCpuCount = nTotalCpuCount;
    m_pcLoadData = pLoadData;
}

//=====================================================================================================================
/**
 * @brief   LoadGetThread::doWork
 *          負荷率を取得し完了したらシグナルを発行する
 * @return  なし
 */
void LoadGetThread::doWork(quint64 nInterval)
{
    QStringList lstLoad    = m_pcLoadData->loadData();
    QString     strCpuName = "^cpu";

    if (m_nCpuId != 0) {
        strCpuName += QString::number(m_nCpuId - 1);
    }

    strCpuName += "\\s.*";
    int nIndex = lstLoad.indexOf(QRegExp(strCpuName, Qt::CaseInsensitive));
    qreal nCalcLoad = 0.0f;

    if (nIndex != -1) {
        QString strLine = lstLoad.at(nIndex);
        QStringList lstToken = strLine.split(QRegExp("\\s"));

        if (lstToken.length() >= 4) {
            quint64 nNowLoad = lstToken[1].toULongLong() + lstToken[2].toULongLong() + lstToken[3].toULongLong();

            if (nInterval != 0) {
                nCalcLoad = static_cast<qreal>(nNowLoad - m_nPrevLoad) / nInterval * 100.0f;
            }

            if (m_nCpuId == 0) {
                nCalcLoad /= m_nTotalCpuCount;
            }

            m_nPrevLoad = nNowLoad;

            if (nInterval != 0) {
                emit finished(m_nCpuId, nCalcLoad);
            }
        } else {
            qDebug() << "stat data failed.";
        }
    }
}
