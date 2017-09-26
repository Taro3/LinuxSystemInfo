#include "cputemperate.h"

//=====================================================================================================================
/**
 * @brief CpuTemperate::instance
 * @return
 */
CpuTemperate *CpuTemperate::instance()
{
    static CpuTemperate cpuTemperate;

    return &cpuTemperate;
}

//=====================================================================================================================
/**
 * @brief CpuTemperate::CpuTemperate
 */
CpuTemperate::CpuTemperate() : QObject()
{
}
