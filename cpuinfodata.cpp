#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QDebug>
#include <QTextStream>

#include "cpuinfodata.h"
#include "osproc.h"

const QString CpuinfoData::PROC_PATH = "/proc/";
const QString CpuinfoData::CPUINFO_FILE_NAME = "cpuinfo";

//=====================================================================================================================
/**
 * @brief CpuinfoData::instance
 * @return
 */
CpuinfoData *CpuinfoData::instance()
{
    static CpuinfoData cpuinfoData;

    return &cpuinfoData;
}

//=====================================================================================================================
/**
 * @brief   CpuinfoData::~CpuinfoData
 *          デストラクタ
 */
CpuinfoData::~CpuinfoData()
{
    delete m_pcLock;           m_pcLock           = nullptr;
    delete m_pclstCpuInfoData; m_pclstCpuInfoData = nullptr;
}

//=====================================================================================================================
/**
 * @brief CpuinfoData::cpuinfoData
 * @return
 */
QStringList CpuinfoData::cpuinfoData()
{
    QReadLocker lock(m_pcLock);
    return *m_pclstCpuInfoData;
}

//=====================================================================================================================
/**
 * @brief   CpuinfoData::processorCount
 *          論理CPU数を返す
 * @return  論理CPU数
 */
int CpuinfoData::processorCount()
{
    const static QString STR_PROCESSOR = "^processor\\s:\\s\\d.*";
    const static QString FILE_PATH     = PROC_PATH + CPUINFO_FILE_NAME;

    if (m_nLogicalCpuCount!= 0) {
        return m_nLogicalCpuCount;
    }

    m_nLogicalCpuCount = 0;

    int nIndex = 0;
    while ((nIndex = m_pclstCpuInfoData->indexOf(QRegExp(STR_PROCESSOR, Qt::CaseInsensitive), nIndex)) >= 0) {
        ++m_nLogicalCpuCount;
    }

    return m_nLogicalCpuCount;
}

//=====================================================================================================================
int CpuinfoData::coreCount()
{
    const static QString STR_CPU_CORES = "^cpu\\scores\\s:\\s\\d.*";

    if (m_nCoreCount != 0) {
        return m_nCoreCount;
    }

    int nIndex = m_pclstCpuInfoData->indexOf(QRegExp(STR_CPU_CORES, Qt::CaseInsensitive));

    if (nIndex < 0) {
        qDebug() << "cpu cores failed.";

        return 0;
    }

    QString strLine = m_pclstCpuInfoData->at(nIndex);
    QString strNum = strLine.right(strLine.length() - strLine.indexOf(':'));
    strNum = strNum.trimmed();
    QTextStream stream(&strNum);
    stream >> m_nCoreCount;

    return m_nCoreCount;
}

//=====================================================================================================================
/**
 * @brief   CpuinfoData::CpuinfoData
 *          コンストラクタ
 */
CpuinfoData::CpuinfoData()
    : QObject()
    , m_pcLock(nullptr)
    , m_pclstCpuInfoData(nullptr)
    , m_nLogicalCpuCount(0)
    , m_nCoreCount(0)
{
    m_pcLock           = new QReadWriteLock;
    m_pclstCpuInfoData = new QStringList;
    QWriteLocker lock(m_pcLock);
    *m_pclstCpuInfoData = OsProc::instance()->loadCpuinfoFile();
    Q_ASSERT(!m_pclstCpuInfoData->isEmpty());
}
