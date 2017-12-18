#include <iostream>
#include <thread>
#include <chrono>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent>
#include <QTimer>

#include "node.h"
#include "uv.h"

#include "RssFeed.h"

int main(int argc, char* argv[]) {
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

    node::lib::initialize();
    node::lib::loadScript("rss_feeds.js");

    QTimer nodeTickTimer();
    QObject::connect(&nodeTickTimer, &QTimer::timeout, [](){ node::lib::processEvents(); });
    nodeTickTimer.start(20);

    app.exec();

    node::lib::terminate();
}
