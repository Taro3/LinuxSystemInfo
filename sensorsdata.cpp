#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

#include "sensorsdata.h"

//=====================================================================================================================
/**
 * @brief SensorsData::SensorsData
 * @param parent
 */
SensorsData::SensorsData(QObject *parent)
    : QObject(parent)
    , m_pcLock(nullptr)
    , m_pclstData(nullptr)
{
    m_pcLock    = new QReadWriteLock;
    m_pclstData = new QStringList;
}

//=====================================================================================================================
/**
 * @brief SensorsData::~SensorsData
 */
SensorsData::~SensorsData()
{
    delete m_pcLock;    m_pcLock    = nullptr;
    delete m_pclstData; m_pclstData = nullptr;
}

//=====================================================================================================================
/**
 * @brief SensorsData::setSecsorsData
 * @param lstSensorData
 */
void SensorsData::setSecsorsData(const QStringList &lstSensorData)
{
    QWriteLocker cLock(m_pcLock);
    *m_pclstData = lstSensorData;
}

//=====================================================================================================================
/**
 * @brief SensorsData::sensorsData
 * @return
 */
QStringList SensorsData::sensorsData() const
{
    QReadLocker cLock(m_pcLock);

    return *m_pclstData;
}
