#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include "cpuload.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

//    qmlRegisterType<CpuLoad>("io.qt.CpuLoad", 1, 0, "CpuLoad");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    CpuLoad cpuLoad;
    cpuLoad.initialize();
    QObject *pRepeaterName = engine.rootObjects().first()->findChild<QObject*>("cpuLoadCpu");
    pRepeaterName->setProperty("model", cpuLoad.cpuCount() + 1);
    engine.rootContext()->setContextProperty("cpuLoad", &cpuLoad);

    return app.exec();
}
