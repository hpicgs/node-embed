#include <iostream>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "RssFeed.h"

int main(int argc, char* argv[]) {
  // Initialize Qt:
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  // to be able to access the public slots of the RssFeed instance
  // we inject a pointer to it in the QML context:
  engine.rootContext()->setContextProperty("rssFeed", &RssFeed::getInstance());

  engine.load(QUrl(QLatin1String("qrc:/main.qml")));
  if (engine.rootObjects().isEmpty()) {
    qWarning() << "Error while loading QML file";
    return 1;
  }

  // Run the Qt application:
  app.exec();
}
