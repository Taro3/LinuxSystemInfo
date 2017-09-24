#ifndef LOADGETTHREAD_H
#define LOADGETTHREAD_H

#include <QObject>
#include <QThread>

class LoadData;

//=====================================================================================================================
/**
 * @brief   The LoadGetThread class
 *          負荷率取得スレッドオブジェクト
 */
class LoadGetThread : public QObject
{
    Q_OBJECT

public:
    explicit LoadGetThread(LoadData * const pLoadData, const int nCpuId, const int nTotalCpuCount
                           , QObject *parent = nullptr);
    //=================================================================================================================
    /**
     * @brief   quit
     *          スレッド終了処理
     * @return  なし
     */
    void quit()
    {
        thread()->exit();
        thread()->wait();
    }

signals:
    //=================================================================================================================
    /**
     * @brief       finished
     *              負荷率取得完了シグナル
     * @param[in]   nCpuIndex   CPUインデックス
     * @arg         0   CPU全体
     * @arg         1~  論理CPU番号+1
     * @param[in]   nLoad   負荷率値
     * @return      なし
     */
    void finished(const int nCpuIndex, const qreal nLoad);

public slots:
    void doWork(quint64 nInterval);

private:
    int         m_nCpuId;           //!< CPU ID
    int         m_nTotalCpuCount;   //!< 総論理CPU数
    LoadData    *m_pcLoadData;      //!< 負荷率データテキスト保持オブジェクト
    quint64     m_nPrevLoad;        //!< 前回の負荷値
};

#endif // LOADGETTHREAD_H
