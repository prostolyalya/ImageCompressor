#include <QGuiApplication>
#include "include/controller.h"
#include <iostream>
#include <QThread>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);
    char *path = argv[0];
    if (argc > 1)
    {
        path = argv[1];
    }
    QQmlApplicationEngine engine;
    qRegisterMetaType<QList<File>>("QList<File>");
    auto controller = std::make_unique<Controller>(path, engine);
    auto thread = new QThread();
    QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    controller->moveToThread(thread);
    thread->start();
    const QUrl url(QStringLiteral("qrc:/ui/main.qml"));
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);
    return app.exec();
}
