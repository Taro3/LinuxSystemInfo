#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "cpuload.h"
#include "cputemperature.h"
#include "osproc.h"
#include "cpuinfodata.h"

//=====================================================================================================================
/**
 * @brief       main
 *              アプリケーションメイン関数
 * @param[in]   argc    引数数
 * @param[in]   argv    引数文字列
 * @return      アプリケーション終了ステータス
 */
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    CpuLoad::instance()->initialize();
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("cpuLoad", CpuLoad::instance());
    engine.rootContext()->setContextProperty("cpuTemperature", CpuTemperature::instance());
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    QObject *pCpuLoadRepeater = engine.rootObjects().first()->findChild<QObject*>("cpuLoadCpu");
    pCpuLoadRepeater->setProperty("model", CpuinfoData::instance()->processorCount() + 1);
    QObject *pCpuTemperatureRepeater = engine.rootObjects().first()->findChild<QObject*>("cpuTemperatureCpu");
    pCpuTemperatureRepeater->setProperty("model", CpuinfoData::instance()->coreCount() + 1);

    return app.exec();
}
