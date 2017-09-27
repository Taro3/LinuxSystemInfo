#include <QObject>

class QFile;

//=====================================================================================================================
/**
 * @brief   The OsProc class
 *          /proc内の情報取得クラス
 * @note    シングルトン実装
 */
class OsProc : QObject
{
    Q_OBJECT

public:
    OsProc(const OsProc &) = delete;                // コピーコンストラクタ無効化
    OsProc& operator =(const OsProc &) = delete;    // 代入演算子無効化
    OsProc(OsProc &&) = delete;                     // 右辺値参照コピーコンストラクタ無効化
    OsProc& operator =(OsProc &&) = delete;         // 右辺値参照代入演算子無効化
    virtual ~OsProc();
    static OsProc* instance();
    int sensorsPackageCount();
    int sensorsCoreCount(const int nPackageId);
    QStringList loadStatFile();
    QStringList loadCpuinfoFile();
    QStringList createSensorsData();

private:
    static const QString PROC_PATH;         //!< /procパス
    static const QString CPUINFO_FILE_NAME; //!< cpuinfoファイル名
    static const QString STAT_FILE_NAME;    //!< statファイル名

    OsProc();                               // デフォルトコンストラクタはアクセス禁止
    QFile*  openOsFile(const QString strFilename);
    int     packageIdIndex(const int nStartIndex);
    QStringList packageSet(const int nPackageId);
};
