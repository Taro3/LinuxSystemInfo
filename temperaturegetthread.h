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
    int         m_nCpuIndex;        //!< CPUインデックス
    SensorsData *m_pcSensorsData;   //!< CPU温度データ
};

#endif // TEMPERATUREGETTHREAD_H
