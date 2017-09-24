#ifndef LOADDATA_H
#define LOADDATA_H

#include <QObject>
#include <QReadWriteLock>
#include <QReadLocker>

//=====================================================================================================================
/**
 * @brief   The LoadData class
 *          CPU負荷率データ保持クラス
 * @note    複数のスレッドからのアクセスに対応する必要がある
 */
class LoadData : public QObject
{
    Q_OBJECT
public:
    explicit LoadData(QObject *parent = nullptr);
    ~LoadData();
    void setLoadData(const QStringList &lst);
    //=================================================================================================================
    /**
     * @brief   loadData
     *          負荷率データテキスト取得
     * @return  負荷率データテキスト
     */
    inline QStringList loadData() const
    {
        QReadLocker lock(m_pcLock);
        return *m_plstLoadData;
    }

signals:

public slots:

private:
    QReadWriteLock  *m_pcLock;          //!< 排他処理用ミューテックス
    QStringList     *m_plstLoadData;    //!< 負荷率データテキスト
};

#endif // LOADDATA_H
