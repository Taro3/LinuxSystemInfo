#ifndef TEMPERATUREGETTHREAD_H
#define TEMPERATUREGETTHREAD_H

#include <QObject>

class SensorsData;

//=====================================================================================================================
/**
 * @brief   The TemperatureGetThread class
 *          CPU温度取得スレッド
 */
class TemperatureGetThread : public QObject
{
    Q_OBJECT

public:
    explicit TemperatureGetThread(SensorsData *pSensorsData, const int nCpuIndex);
    void quit();

signals:
    void finished(const int nCpuIndex, const qreal nCpuTemperature);

public slots:
    void doWork();

private:
    int         m_nCpuIndex;
    SensorsData *m_pcSensorsData;
};

#endif // TEMPERATUREGETTHREAD_H
