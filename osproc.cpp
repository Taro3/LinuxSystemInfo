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
{
}

//=====================================================================================================================
/**
 * @brief OsProc::openOsFile
 * @param strFilename
 * @return
 */
QFile *OsProc::openOsFile(const QString strFilename)
{
    QFile *pFile = new QFile(strFilename);

    if (!pFile->exists()) {
        qDebug() << strFilename + " not found.";
        delete pFile;

        return nullptr;
    }

    if (!pFile->open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << strFilename + " can't open.";
        delete pFile;

        return nullptr;
    }

    return pFile;
}

//=====================================================================================================================
/**
 * @brief OsProc::packageIdIndex
 * @param nStartIndex
 * @return
 */
int OsProc::packageIdIndex(const int nStartIndex)
{
    const static QString STR_PACKAGE_ID = "^Package\\sid\\s\\d:.*";

    QStringList lst           = createSensorsData();

    return lst.indexOf(QRegExp(STR_PACKAGE_ID, Qt::CaseInsensitive), nStartIndex);
}

//=====================================================================================================================
QStringList OsProc::packageSet(const int nPackageId)
{
    const static QString STR_PACKAGE_ID = "^Package\\sid\\s%1:.*";

    QStringList lstData = createSensorsData();
    QStringList lst;
    QString strFind = STR_PACKAGE_ID.arg(nPackageId);
    int nIndex = lstData.indexOf(QRegExp(strFind, Qt::CaseInsensitive));

    if (nIndex < 0) {
        return lst;
    }

    for (int i = nIndex; nIndex < lstData.count(); ++i) {
        QString strLine = lstData.at(i);
        strLine = strLine.trimmed();
        if (strLine.isEmpty()) {
            break;
        }
        lst.append(strLine);
    }

    return lst;
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
}

//=====================================================================================================================
/**
 * @brief   OsProc::sensorsPackageCount
 *          cpuiinfo内のPackage id数を返す
 * @return  Package id数
 */
int OsProc::sensorsPackageCount()
{
    int nPackageCount = 0;
    int nIndex        = 0;

    while ((nIndex = packageIdIndex(nIndex)) >= 0) {
        ++nPackageCount;
    }

    return nPackageCount;
}

//=====================================================================================================================
int OsProc::sensorsCoreCount(const int nPackageId)
{
    QStringList lst;

    lst = packageSet(nPackageId);

    return lst.count() - 1;
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

    QFile *pFile = openOsFile(FILE_PATH);

    if (pFile == nullptr) {
        return lstLoad;
    }

    char szLineBuffer[1024];

    while (pFile->readLine(szLineBuffer, sizeof(szLineBuffer)) != -1) {
        lstLoad.append(szLineBuffer);
    }

    pFile->close();
    delete pFile; pFile = nullptr;

    return lstLoad;
}

//=====================================================================================================================
/**
 * @brief   OsProc::loadCpuinfoFile
 *          CPU情報ファイル取得処理
 * @return  CPU情報データテキスト
 */
QStringList OsProc::loadCpuinfoFile()
{
    const static QString FILE_PATH = PROC_PATH + CPUINFO_FILE_NAME;

    QStringList lstCpuinfo;

    QFile *pFile = openOsFile(FILE_PATH);

    if (!pFile) {
        return lstCpuinfo;
    }

    char szLineBuffer[1024];

    while (pFile->readLine(szLineBuffer, sizeof(szLineBuffer)) >= 0) {
        lstCpuinfo.append(szLineBuffer);
    }

    pFile->close();
    delete pFile; pFile = nullptr;

    return lstCpuinfo;
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

    QProcess    cProcess;
    QStringList lstSensors;

    cProcess.start(SENSORS_COMMAND);
    cProcess.waitForFinished();

    QString strLine;

    while (!(strLine = cProcess.readLine()).isEmpty()) {
        lstSensors.append(strLine);
    }

    return lstSensors;
}
