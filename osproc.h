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
    static OsProc* createInstance(QObject *parent = nullptr);
    static OsProc* instance();
    int cpuInfoProcessorCount();
    int cpuInfoCoreCount();
    QStringList loadStatFile();
    QStringList createSensorsData();
    QList<quint64> statCpu(int nIndex);
    QList<QList<quint64> > statCpus();

private:
    static const QString PROC_PATH;         //!< /procパス
    static const QString CPUINFO_FILE_NAME; //!< cpuinfoファイル名
    static const QString STAT_FILE_NAME;    //!< statファイル名
    QFile   *m_pcFileCpuInfo;               //!< cpuinfo用ファイルオブジェクト
    QFile   *m_pcFileStat;                  //!< stat用ファイルオブジェクト
    int     m_nCpuLogicalCount;             //!< 論理CPU数
    int     m_nCpuCoreCount;                //!< CPUコア数

    OsProc();                               // デフォルトコンストラクタはアクセス禁止
};
