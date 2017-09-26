#ifndef TEMPERATUREGETTHREAD_H
#define TEMPERATUREGETTHREAD_H

#include <QObject>

//=====================================================================================================================
/**
 * @brief   The TemperatureGetThread class
 *          CPU温度取得スレッド
 */
class TemperatureGetThread : public QObject
{
    Q_OBJECT

public:
    explicit TemperatureGetThread(const int nCpuIndex);
    void quit();

signals:
    void finished(const int nCpuIndex, const quint64 nCpuTemperature);

public slots:
    void doWork();

private:
    int m_nCpuIndex;
};

#endif // TEMPERATUREGETTHREAD_H
