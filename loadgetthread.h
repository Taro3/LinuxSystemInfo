#ifndef LOADGETTHREAD_H
#define LOADGETTHREAD_H

#include <QObject>

class LoadData;

/**
 * @brief   The LoadGetThread class
 *          負荷率取得スレッドオブジェクト
 */
class LoadGetThread : public QObject
{
    Q_OBJECT
public:
    explicit LoadGetThread(LoadData * const pLoadData, const int nCpuId, QObject *parent = nullptr);

signals:
    void finished(const int nCpuIndex, qreal nLoad);

public slots:
    void doWork();

private:
    int      m_nCpuId;
    LoadData *m_pcLoadData;
};

#endif // LOADGETTHREAD_H
