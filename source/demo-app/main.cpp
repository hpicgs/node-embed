#include <iostream>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent>
#include <QTimer>

#include "node.h"

#include "RssFeed.h"

v8::FunctionCallbackInfo<v8::Value>* nodeExitInfo = nullptr;
bool guiClosed = false;

void shouldExit(const v8::FunctionCallbackInfo<v8::Value>& args) {
    args.GetReturnValue().Set(guiClosed);
}

void nodeRunExitFunc(){
    if (nodeExitInfo == nullptr) {
        return;
    }
    v8::Isolate* isolate = nodeExitInfo->GetIsolate();
    v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast((*nodeExitInfo)[0]);
    //const unsigned argc = 1;
    v8::Local<v8::Value> argv[0] = {};
    //cb->Call(v8::Null(isolate), 0, argv);
    cb->Call(v8::Null(isolate), 0, argv);
}
void registerExitFunc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    nodeExitInfo = new v8::FunctionCallbackInfo<v8::Value>(args);
    //nodeRunExitFunc();
}

void processQtEvents(const v8::FunctionCallbackInfo<v8::Value>&) {
    QCoreApplication::processEvents();
}

void onRegisterModule(v8::Local<v8::Object> exports, v8::Local<v8::Value>, v8::Local<v8::Context>, void * data)
{
    NODE_SET_METHOD(exports, "cppLog", RssFeed::cppLog);
    NODE_SET_METHOD(exports, "registerExitFunc", registerExitFunc);
    NODE_SET_METHOD(exports, "shouldExit", shouldExit);
    NODE_SET_METHOD(exports, "clearFeed", RssFeed::clearFeed);
    NODE_SET_METHOD(exports, "redrawGUI", RssFeed::redrawGUI);
    NODE_SET_METHOD(exports, "processQtEvents", processQtEvents);
}


int main(int argc, char* argv[])
{
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

    // Create node module
    static node::node_module _rssModule =
    {
        NODE_MODULE_VERSION,
        NM_F_BUILTIN,
        nullptr,
        __FILE__,
        nullptr,
        onRegisterModule,
        "CppDemoModule",
        nullptr, // nm_priv, will be given to onRegisterModule as data? i.e. "this" pointer
        nullptr
    };

    // Register module
    node_module_register(&_rssModule);

    QObject::connect(&engine, &QQmlEngine::quit, [](){ guiClosed = true; });
    node::Start(argc, argv);
}
