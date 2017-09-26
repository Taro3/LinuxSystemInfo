#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "cpuload.h"
#include "osproc.h"

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
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    QObject *pRepeaterName = engine.rootObjects().first()->findChild<QObject*>("cpuLoadCpu");
    pRepeaterName->setProperty("model", OsProc::instance()->cpuInfoProcessorCount() + 1);

    return app.exec();
}
