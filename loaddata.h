#ifndef LOADDATA_H
#define LOADDATA_H

#include <QObject>
#include <QMutex>

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
    void setLoadData(QStringList &lst);
    void lock();
    void unlock();
    inline QStringList loadData() const
    {
        return m_lstLoadData;
    }

signals:

public slots:

private:
    QMutex  m_cMutex;
    QStringList m_lstLoadData;
};

#endif // LOADDATA_H
