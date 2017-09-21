#ifndef CPULOAD_H
#define CPULOAD_H

#include <QObject>
#include <QVector>

class OsProc;
class LoadData;
class LoadGetThread;

/**
 * @brief   The CpuLoad class
 *          CPU使用率取得用クラス
 */
class CpuLoad : public QObject
{
    Q_OBJECT

public:
    explicit CpuLoad(QObject *parent = nullptr);
    virtual ~CpuLoad();
    bool initialize();
    Q_INVOKABLE QList<qreal> cpuLoad();
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
    void loadUpdated(const int nIndex, const qreal nLoad);

public slots:
    void getLoadFinished(const int nCpuIndex, const qreal load);

private:
    typedef struct {
        clock_t         prevClock;              //!< 前回取得時刻
        QList<quint64>  lstInfo;                //!< 取得CPU時間
    } sInfoAndTime;

    OsProc      *m_pcOsProc;                    //!< OsProcオブジェクト
    LoadData    *m_pcLoadData;
    int         m_nProcessorCount;              //!< 論理CPU数
    bool        m_isReady;                      //!< 初期化完了フラグ
    QList<LoadGetThread*>   m_lstGetThread;
    QVector<sInfoAndTime> m_vecCpuInfo;         //!< CPU情報
};

#endif // CPULOAD_H
