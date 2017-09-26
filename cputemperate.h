#ifndef CPUTEMPERATE_H
#define CPUTEMPERATE_H

#include <QObject>

//=====================================================================================================================
/**
 * @brief   The CpuTemperate class
 *          CPU温度取得クラス
 * @note    シングルトン実装
 */
class CpuTemperate : public QObject
{
    Q_OBJECT

public:
    static CpuTemperate* instance();
    CpuTemperate(const CpuTemperate&) = delete;                // コピーコンストラクタ無効化
    CpuTemperate& operator =(const CpuTemperate &) = delete;    // 代入演算子無効化
    CpuTemperate(CpuTemperate &&) = delete;                     // 右辺値参照コピーコンストラクタ無効化
    CpuTemperate& operator =(CpuTemperate &&) = delete;         // 右辺値参照代入演算子無効化

signals:

public slots:

private:
    CpuTemperate();
};

#endif // CPUTEMPERATE_H
