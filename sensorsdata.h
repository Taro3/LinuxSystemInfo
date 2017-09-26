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
    QReadWriteLock  *m_pcLock;
    QStringList     *m_pclstData;
};

#endif // SENSORSDATA_H
