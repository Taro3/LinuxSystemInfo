#ifndef CPUTEMPERATURE_H
#define CPUTEMPERATURE_H

#include <QObject>

class TemperatureGetThread;
class SensorsData;

//=====================================================================================================================
/**
 * @brief   The CpuTemperature class
 *          CPU温度取得クラス
 * @note    シングルトン実装
 */
class CpuTemperature : public QObject
{
    Q_OBJECT

public:
    static CpuTemperature* instance();
    virtual ~CpuTemperature();
    CpuTemperature(const CpuTemperature&) = delete;                 // コピーコンストラクタ無効化
    CpuTemperature& operator =(const CpuTemperature &) = delete;    // 代入演算子無効化
    CpuTemperature(CpuTemperature &&) = delete;                     // 右辺値参照コピーコンストラクタ無効化
    CpuTemperature& operator =(CpuTemperature &&) = delete;         // 右辺値参照代入演算子無効化
    Q_INVOKABLE void startGetCpuTemperature();

signals:
    void temperatureUpdated(const int nCpuIndex, const qreal nTemperature);

public slots:
    void getTemperatureFinished(const int nCpuIndex, const qreal nTemperature);

private:
    SensorsData                     *m_pcSendorsData;
    QList <TemperatureGetThread*>   m_clstThreadl;

    CpuTemperature();
};

#endif // CPUTEMPERATURE_H
