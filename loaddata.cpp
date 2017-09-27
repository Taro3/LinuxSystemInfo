#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>

#include "loaddata.h"

//=====================================================================================================================
/**
 * @brief       LoadData::LoadData
 *              コンストラクタ
 * @param[in]   parent  親オブジェクトポインタ
 * @return      なし
 */
LoadData::LoadData(QObject *parent)
    : QObject(parent)
    , m_pcLock(nullptr)
    , m_plstLoadData(nullptr)
{
    m_pcLock       = new QReadWriteLock;
    m_plstLoadData = new QStringList;
}

//=====================================================================================================================
/**
 * @brief   LoadData::~LoadData
 *          デストラクタ
 */
LoadData::~LoadData()
{
    delete m_pcLock;       m_pcLock       = nullptr;
    delete m_plstLoadData; m_plstLoadData = nullptr;
}

//=====================================================================================================================
/**
 * @brief       LoadData::setLoadData
 *              負荷率データテキスト設定
 * @param[in]   lst 負荷率データテキスト
 * @return      なし
 */
void LoadData::setLoadData(const QStringList &lst)
{
    QWriteLocker wLock(m_pcLock);

    *m_plstLoadData = lst;
}
