#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTimer>

#include "node.h"
#include "node_lib.h"

#include "RssFeed.h"

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // to be able to access the public slots of the RssFeed instance
    // we inject a pointer to it in the QML context:
    RssFeed* rssFeed = new RssFeed();
    engine.rootContext()->setContextProperty("rssFeed", rssFeed);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Error while loading QML file";
        return -1;
    }

    node::lib::Initialize("rss_feed_demo_app");
    node::lib::Run("rss_feeds.js");
    while (node::lib::ProcessEvents()) { }

    QTimer nodeTickTimer();
    QObject::connect(&nodeTickTimer, &QTimer::timeout, this, [](){ node::lib::ProcessEvents(); });
    nodeTickTimer.start(20);

    app.exec();

    node::lib::Terminate();
}
