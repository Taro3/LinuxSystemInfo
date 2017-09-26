#ifndef CPULOAD_H
#define CPULOAD_H

#include <QObject>
#include <QVector>
#include <sys/times.h>

class OsProc;
class LoadData;
class LoadGetThread;

//=====================================================================================================================
/**
 * @brief   The CpuLoad class
 *          CPU使用率取得用クラス
 * @note    シングルトン実装
 */
class CpuLoad : public QObject
{
    Q_OBJECT

public:
    static CpuLoad* instance();
    CpuLoad(const CpuLoad &) = delete;                // コピーコンストラクタ無効化
    CpuLoad& operator =(const CpuLoad &) = delete;    // 代入演算子無効化
    CpuLoad(CpuLoad &&) = delete;                     // 右辺値参照コピーコンストラクタ無効化
    CpuLoad& operator =(CpuLoad &&) = delete;         // 右辺値参照代入演算子無効化
    virtual ~CpuLoad();
    bool initialize();
    //=================================================================================================================
    /**
     * @brief   cpuCount
     *          論理CPU数取得
     * @return  CPU数(エラーの場合は0を返す)
     * @note    QMLから呼び出し可能
     */
    Q_INVOKABLE inline int cpuCount()
    {
        return m_nProcessorCount;
    }
    //=================================================================================================================
    /**
     * @brief   isReady
     *          初期化完了確認
     * @return  初期化完了状態
     * @retval  true    初期化済み
     * @retval  false   未初期化
     */
    Q_INVOKABLE inline bool isReady() const
    {
        return m_isReady;
    }
    Q_INVOKABLE void startGetCpuLoad();

signals:
    //=================================================================================================================
    /**
     * @brief       loadUpdated
     *              負荷率取得完了通知シグナル
     * @param[inb]  nIndex  CPUインデックス
     * @arg         0   CPU全体
     * @arg         1~  各論理CPU番号+1
     * @param[in]   nLoad   負荷率値
     * @return      なし
     */
    void loadUpdated(const int nIndex, const qreal nLoad);

public slots:
    void getLoadFinished(const int nCpuIndex, const qreal load);

private:
    LoadData    *m_pcLoadData;                  //!< 負荷率テキストデータ保持オブジェクト
    int         m_nProcessorCount;              //!< 論理CPU数
    bool        m_isReady;                      //!< 初期化完了フラグ
    clock_t     m_nPrevClock;                   //!< 前回の負荷率取得時クロック
    QList<LoadGetThread*>   m_lstGetThread;     //!< 負荷率取得スレッドリスト

    CpuLoad();
};

#endif // CPULOAD_H
