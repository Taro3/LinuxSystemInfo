#include <QObject>

class QFile;

/**
 * @brief   The OsProc class
 *          /proc内の情報取得クラス
 */
class OsProc : QObject
{
    Q_OBJECT
public:
    explicit OsProc(QObject *parent);
    virtual ~OsProc();
    int cpuInfoProcessorCount();
    QStringList loadStatFile();
    QList<quint64> statCpu(int nIndex);
    QList<QList<quint64> > statCpus();

private:
    static const QString PROC_PATH;         //!< /procパス
    static const QString CPUINFO_FILE_NAME; //!< cpuinfoファイル名
    static const QString STAT_FILE_NAME;    //!< statファイル名
    QFile   *m_pcFileCpuInfo;               //!< cpuinfo用ファイルオブジェクト
    QFile   *m_pcFileStat;                  //!< stat用ファイルオブジェクト
};
