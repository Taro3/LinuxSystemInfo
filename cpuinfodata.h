#ifndef CPUINFODATA_H
#define CPUINFODATA_H

#include <QObject>

class QReadWriteLock;

//=====================================================================================================================
/**
 * @brief   The CpuinfoData class
 *          cpuinfoデータ文字列保持クラス
 * @note    シングルトン実装
 */
class CpuinfoData : public QObject
{
    Q_OBJECT
public:
    static CpuinfoData* instance();
    CpuinfoData(const CpuinfoData &) = delete;              // コピーコンストラクタ無効化
    CpuinfoData& operator =(const CpuinfoData &) = delete;  // 代入演算子無効化
    CpuinfoData(CpuinfoData &&) = delete;                   // 右辺値参照コピーコンストラクタ無効化
    CpuinfoData& operator =(CpuinfoData &&) = delete;       // 右辺値参照代入演算子無効化
    virtual ~CpuinfoData();
    QStringList cpuinfoData();
    int processorCount();
    int coreCount();

signals:

public slots:

private:
    static const QString    PROC_PATH;
    static const QString    CPUINFO_FILE_NAME;

    QReadWriteLock  *m_pcLock;              //!< 排他処理用ロックオブジェクト
    QStringList     *m_pclstCpuInfoData;    //!< cpuinfoデータオブジェクト
    int             m_nLogicalCpuCount;     //!< 論理CPU数
    int             m_nCoreCount;           //!< コア数

    CpuinfoData();
};

#endif // CPUINFODATA_H
