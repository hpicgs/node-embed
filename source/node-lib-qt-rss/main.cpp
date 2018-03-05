#include <chrono>
#include <iostream>
#include <thread>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <cpplocate/cpplocate.h>

#include "node.h"
#include "node_lib.h"

#include "RssFeed.h"

int main(int argc, char* argv[]) {
  // Locate the JavaScript file we want to load:
  const std::string js_file = "data/node-lib-qt-rss.js";
  const std::string data_path = cpplocate::locatePath(js_file);
  if (data_path.empty()) {
    qWarning() << "Could not find data path.";
    return 1;
  }
  const std::string js_path = data_path + "/" + js_file;

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

  // Initialize Node.js engine:
  node::Initialize();

  // Register C++ methods to be used within JavaScript
  // The third parameter binds the C++ module to that name,
  // allowing the functions to be called like this: "cppQtGui.clearFeed(...)"
  node::RegisterModule("cpp-qt-gui", {
                         {"addFeedItem", RssFeed::addFeedItem},
                         {"clearFeed", RssFeed::clearFeed},
                       }, "cppQtGui");

  // Evaluate the JavaScript file once:
  node::Run(js_path);

  // Load intial RSS feed to display it:
  RssFeed::refreshFeed();

  // Run the Qt application:
  app.exec();

  // After we are done, deinitialize the Node.js engine:
  node::Deinitialize();
}
