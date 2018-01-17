#include <iostream>
#include <thread>
#include <chrono>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "node.h"
#include "node_lib.h"

#include "RssFeed.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        qFatal("Provide script path!");
        return -1;
    }

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // to be able to access the public slots of the RssFeed instance
    // we inject a pointer to it in the QML context:
    engine.rootContext()->setContextProperty("rssFeed", &RssFeed::getInstance());

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Error while loading QML file";
        return -1;
    }

    QObject::connect(&engine, &QQmlEngine::quit, [](){ node::lib::RequestStopEventLoop(); });

    node::lib::Initialize();
    node::lib::RegisterModule("CppDemoModule", {
                                {"cppLog", RssFeed::cppLog},
                                {"clearFeed", RssFeed::clearFeed},
                                {"redrawGUI", RssFeed::redrawGUI},
                              });
    node::lib::Run(argv[1]);
    node::lib::RunEventLoop([](){ QCoreApplication::processEvents(); });
    //node::lib::Deinitialize();
}
