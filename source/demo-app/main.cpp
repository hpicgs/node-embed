#include <iostream>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent>

#include "node.h"

#include "RssFeed.h"

v8::FunctionCallbackInfo<v8::Value>* nodeExitInfo = nullptr;
bool guiClosed = false;

// callback for Node.js to check if the GUI was closed and therefore node should exit. Returns guiClosed to node.js
void shouldExit(const v8::FunctionCallbackInfo<v8::Value>& args) {
    args.GetReturnValue().Set(guiClosed);
}

// calls node-function registered in registerExitInfo(), is supposed to be called once the node.js-environment is
// supposed to be killed (i.e. C++-code is done). Before calling this, Node needs to register it's exit function. TODO: fixme
void nodeRunExitFunc(){
    if (nodeExitInfo == nullptr) {
        // this should never be reached when nodeExitInfo is set. TODO: Throw exception
        return;
    }
    v8::Isolate* isolate = nodeExitInfo->GetIsolate();
    v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast((*nodeExitInfo)[0]);
    //const unsigned argc = 1;
    v8::Local<v8::Value> argv[0] = {};
    // Call() does not work, our theory is that one of the function parameters or the callback object itself
    // goes into invalid state, causing a segfault. While this ends the node-loop, it's not quite what we want here.
    cb->Call(v8::Null(isolate), 0, argv);
}

// callback for Node.js to register the exit-function. The exit-function is supposed to get called once the Qt-GUI
// gets closed, this function should be called on startup of node.js
void registerExitFunc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    nodeExitInfo = new v8::FunctionCallbackInfo<v8::Value>(args);
}

// exposes C++-functions in node.js-Module _rssModule (called CppDemoModule in node.js)
// format is NODE_SET_METHOD(exports, "functionNameInJS", functionPointerInC++).
// From what we understand, object member functions can not be exposed to node.js like this.
void onRegisterModule(v8::Local<v8::Object> exports, v8::Local<v8::Value>, v8::Local<v8::Context>, void * data)
{
    NODE_SET_METHOD(exports, "cppLog", RssFeed::cppLog);
    NODE_SET_METHOD(exports, "registerExitFunc", registerExitFunc);
    NODE_SET_METHOD(exports, "shouldExit", shouldExit);
    NODE_SET_METHOD(exports, "clearFeed", RssFeed::clearFeed);
    NODE_SET_METHOD(exports, "redrawGUI", RssFeed::redrawGUI);
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

    QtConcurrent::run([argc, argv](){ node::Start(argc, argv);});

    app.exec();
    //nodeRunExitFunc();
    guiClosed = true;
}