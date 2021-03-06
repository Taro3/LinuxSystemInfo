#include <QFile>
#include <QDebug>
#include <QStringList>
#include <QProcess>

#include "osproc.h"

const QString OsProc::PROC_PATH         = "/proc/";
const QString OsProc::CPUINFO_FILE_NAME = "cpuinfo";
const QString OsProc::STAT_FILE_NAME    = "stat";

//=====================================================================================================================
/**
 * @brief OsProc::OsProc
 * @param[in]   parent  親オブジェクトポインタ
 * @return      なし
 */
OsProc::OsProc() : QObject()
  , m_pcFileCpuInfo(nullptr)
  , m_pcFileStat(nullptr)
  , m_nCpuLogicalCount(0)
  , m_nCpuCoreCount(0)
{
}

//=====================================================================================================================
/**
 * @brief   OsProc::instance
 *          インスタンス取得
 * @return  インスタンスポインタ
 */
OsProc *OsProc::instance()
{
    static OsProc cOsProc;

    return &cOsProc;
}

//=====================================================================================================================
/**
 * @brief   OsProc::~OsProc
 *          デストラクタ
 */
OsProc::~OsProc()
{
    // オブジェクト破棄
    if (m_pcFileCpuInfo != nullptr) m_pcFileCpuInfo->close();
    if (m_pcFileStat != nullptr)    m_pcFileStat->close();

    delete m_pcFileCpuInfo; m_pcFileCpuInfo = nullptr;
    delete m_pcFileStat;    m_pcFileStat = nullptr;
}

//=====================================================================================================================
/**
 * @brief   OsProc::cpuInfoProcessorCount
 *          論理プロセッサ数取得処理
 * @return  プロセッサ数
 * @note    取得に失敗した場合は0を返す
 */
int OsProc::cpuInfoProcessorCount()
{
    const static QString STR_PROCESSOR = "processor";
    const static QString FILE_PATH = PROC_PATH + CPUINFO_FILE_NAME;

    if (m_nCpuLogicalCount != 0) {
        return m_nCpuLogicalCount;
    }

    if (!m_pcFileCpuInfo)
    {
        m_pcFileCpuInfo = new QFile(FILE_PATH);

        if (!m_pcFileCpuInfo->exists())
        {
            qDebug() << FILE_PATH + " not found.";

            return 0;
        }
    }

    if (!m_pcFileCpuInfo->isOpen())
    {
        if (!m_pcFileCpuInfo->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << FILE_PATH + " can't open.";

            return 0;
        }
    }

    m_nCpuLogicalCount = 0;
    m_pcFileCpuInfo->seek(0);

    static char szLine[1024];

    while (m_pcFileCpuInfo->readLine(szLine, sizeof(szLine))  != -1)
    {
        if (QString(szLine).indexOf(QRegExp(STR_PROCESSOR , Qt::CaseInsensitive)) == 0)
        {
            ++m_nCpuLogicalCount;
        }
    }

    m_pcFileCpuInfo->close();

    return m_nCpuLogicalCount;
}

//=====================================================================================================================
int OsProc::cpuInfoCoreCount()
{
    const static QString STR_CPU_CORES = "^cpu\\scores\\s:";
    const static QString FILE_PATH = PROC_PATH + CPUINFO_FILE_NAME;

    if (m_nCpuCoreCount != 0) {
        return m_nCpuCoreCount;
    }

    if (!m_pcFileCpuInfo)
    {
        m_pcFileCpuInfo = new QFile(FILE_PATH);

        if (!m_pcFileCpuInfo->exists())
        {
            qDebug() << FILE_PATH + " not found.";

            return 0;
        }
    }

    if (!m_pcFileCpuInfo->isOpen())
    {
        if (!m_pcFileCpuInfo->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << FILE_PATH + " can't open.";

            return 0;
        }
    }

    m_pcFileCpuInfo->seek(0);

    static char szLine[1024];

    while (m_pcFileCpuInfo->readLine(szLine, sizeof(szLine))  != -1)
    {
        if (QString(szLine).indexOf(QRegExp(STR_CPU_CORES, Qt::CaseInsensitive)) == 0)
        {
            QStringList lst = QString(szLine).split(':');
            if (lst.count() < 2) {
                qDebug() << "Core count data failed.";
            } else {
                QString strCoreCount = lst[1].trimmed();
                m_nCpuCoreCount = strCoreCount.toInt();
            }
            break;
        }
    }

    m_pcFileCpuInfo->close();

    return m_nCpuCoreCount;
}

//=====================================================================================================================
/**
 * @brief   OsProc::loadStatFile
 *          負荷率データテキスト取得処理
 * @return  負荷率データテキスト
 */
QStringList OsProc::loadStatFile()
{
    const static QString FILE_PATH = PROC_PATH + STAT_FILE_NAME;
    QStringList lstLoad;

    if (!m_pcFileStat)
    {
        m_pcFileStat = new QFile(FILE_PATH);
    }

    if (!m_pcFileStat->exists())
    {
        qDebug() << FILE_PATH + " not found.";

        return lstLoad;
    }

    if (!m_pcFileStat->isOpen())
    {
        if (!m_pcFileStat->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << FILE_PATH + " can't open.";

            return lstLoad;
        }
    }

    m_pcFileStat->seek(0);
    char szLineBuffer[1024];

    while (m_pcFileStat->readLine(szLineBuffer, sizeof(szLineBuffer)) != -1)
    {
        lstLoad.append(szLineBuffer);
    }

    m_pcFileStat->close();

    return lstLoad;
}

//=====================================================================================================================
/**
 * @brief   OsProc::createSensorsData
 *          sensors コマンドを実行しコア温度データ文字列を取得する
 * @return  コア温度データテキスト
 * @note    失敗した場合は空の QStringList を返す
 */
QStringList OsProc::createSensorsData()
{
    static const QString SENSORS_COMMAND = "sensors";

    QProcess cProcess;
    QStringList lstSensors;

    cProcess.start(SENSORS_COMMAND);
    cProcess.waitForFinished();

    QString strLine;

    while (!(strLine = cProcess.readLine()).isEmpty())
    {
        lstSensors.append(strLine);
    }

    return lstSensors;
}

//=====================================================================================================================
/**
 * @brief       OsProc::statCpu
 *              CPU処理時間情報を取得する
 * @param[in]   nIndex  取得する論理プロセッサ番号
 * @arg         -1  CPU全体
 * @arg         0~n 指定のインデックスの論理プロセッサ
 * @return      論理プロセッサの処理時間
 * @note        値の詳細は man proc 内の /proc/stat 参照
 */
QList<quint64> OsProc::statCpu(int nIndex)
{
    const static QString STR_CPU = "cpu";
    const static QString FILE_PATH = PROC_PATH + STAT_FILE_NAME;

    QList<quint64> cpuStat;

    if (!m_pcFileStat)
    {
        m_pcFileStat = new QFile(FILE_PATH);

        if (!m_pcFileStat->exists())
        {
            qDebug() << FILE_PATH + " not found.";

            return cpuStat;
        }
    }

    if (!m_pcFileStat->isOpen())
    {
        if (!m_pcFileStat->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << FILE_PATH + " can't open.";

            return cpuStat;
        }
    }

    m_pcFileStat->seek(0);
    QString strCpuName = "^" + STR_CPU;

    if (nIndex >= 0)
    {
        strCpuName += QString::number(nIndex);
    }

    strCpuName += "\\s";

    static char szLine[1024];

    while (m_pcFileStat->readLine(szLine, sizeof(szLine)) != -1)
    {
        if (QString(szLine).indexOf(QRegExp(strCpuName, Qt::CaseInsensitive)) == 0)
        {
            QTextStream stream(szLine);
            QString strData;
            stream >> strData;

            while (!strData.isEmpty())
            {
                stream >> strData;
                cpuStat << strData.toULongLong();
            }
            break;
        }
    }

    m_pcFileStat->close();

    return cpuStat;
}

//=====================================================================================================================
/**
 * @brief       OsProc::statCpus
 *              すべてのCPUの処理時間情報を取得する
 * @return      処理時間情報(全体1 + 論理プロセッサ数)
 */
QList<QList<quint64> > OsProc::statCpus()
{
    const static QString STR_CPU = "cpu";
    const static QString FILE_PATH = PROC_PATH + STAT_FILE_NAME;

    QList<QList<quint64>> cpuStats;

    if (!m_pcFileStat)
    {
        m_pcFileStat = new QFile(FILE_PATH);

        if (!m_pcFileStat->exists())
        {
            qDebug() << FILE_PATH + " not found.";

            return cpuStats;
        }
    }

    if (!m_pcFileStat->isOpen())
    {
        if (!m_pcFileStat->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << FILE_PATH + " can't open.";

            return cpuStats;
        }
    }

    m_pcFileStat->seek(0);

    static char szLine[1024];
    int index = 0;
    int cpuNo = cpuInfoProcessorCount();

    while (m_pcFileStat->readLine(szLine, sizeof(szLine)) != -1)
    {
        QString strCpuName = "^" + STR_CPU;

        if (index > 0)
        {
            strCpuName += QString::number(index - 1);
        }

        strCpuName += "\\s";

        if (QString(szLine).indexOf(QRegExp(strCpuName, Qt::CaseInsensitive)) == 0)
        {
            QTextStream stream(szLine);
            QString strData;
            stream >> strData;              // CPU名読み飛ばし
            QList<quint64> load;

            while (!strData.isEmpty())
            {
                stream >> strData;
                load << strData.toULongLong();
            }

            cpuStats << load;

            if (++index == cpuNo + 1)
            {
                break;
            }
        }
    }

    m_pcFileStat->close();

    return cpuStats;
}
