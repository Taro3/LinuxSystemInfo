#include <QMutexLocker>

#include "loaddata.h"

LoadData::LoadData(QObject *parent) : QObject(parent)
{

}

void LoadData::setLoadData(QStringList &lst)
{
    QMutexLocker locker(&m_cMutex);

    m_lstLoadData = lst;
}

void LoadData::lock()
{
    m_cMutex.lock();
}

void LoadData::unlock()
{
    m_cMutex.unlock();
}
