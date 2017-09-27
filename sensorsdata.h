#ifndef SENSORSDATA_H
#define SENSORSDATA_H

#include <QObject>

class QReadWriteLock;

//=====================================================================================================================
/**
 * @brief   The SensorsData class
 *          CPU温度データテキスト保持クラス
 */
class SensorsData : public QObject
{
    Q_OBJECT
public:
    explicit SensorsData(QObject *parent = nullptr);
    virtual ~SensorsData();
    void setSecsorsData(const QStringList &lstSensorData);
    QStringList sensorsData() const;

signals:

public slots:

private:
    QReadWriteLock  *m_pcLock;      //!< 排他書利用ロックオブジェクト
    QStringList     *m_pclstData;   //!< CPU温度データテキスト
};

#endif // SENSORSDATA_H
