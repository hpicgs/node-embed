#include <iostream>
#include <thread>
#include <chrono>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include <cpplocate/cpplocate.h>

#include "node.h"
#include "node_lib.h"

#include "RssFeed.h"

int main(int argc, char* argv[]) {
    const std::string js_file = "data/node-lib-qt-rss.js";
    const std::string data_path = cpplocate::locatePath(js_file);
    if (data_path.empty()) {
        std::cerr << "Could not find data path." << std::endl;
        return 1;
    }
    const std::string js_path = data_path + "/" + js_file;

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

    node::lib::Initialize();
    node::lib::RegisterModule("cpp-demo-module", {
                                {"cppLog", RssFeed::cppLog},
                                {"clearFeed", RssFeed::clearFeed},
                                {"redrawGUI", RssFeed::redrawGUI},
                              }, "cppDemoModule");
    node::lib::Run(js_path);
    RssFeed::refreshFeed();
    app.exec();
    node::lib::Deinitialize();
}
