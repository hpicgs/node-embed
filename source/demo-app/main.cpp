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

struct AsyncData {
  uv_async_t request;
  v8::Persistent<v8::Function> callback;
};

AsyncData* jsExitData = nullptr;

void execAsyncCall(uv_async_t* req) {
    AsyncData* data = static_cast<AsyncData*>(req->data);
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);
    v8::Handle<v8::Value> argv[] = { v8::Array::New(isolate) };
    v8::Local<v8::Function>::New(isolate, data->callback)->
          Call(isolate->GetCurrentContext()->Global(), 1, argv);
    data->callback.Reset();
    delete data;
}

// callback for Node.js to register the exit-function. The exit-function is supposed to get called once the Qt-GUI
// gets closed, this function should be called on startup of node.js
void registerExitFunc(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();
    v8::Local<v8::Function> callback = v8::Local<v8::Function>::Cast(args[0]);

    jsExitData = new AsyncData();
    jsExitData->request.data = jsExitData;
    jsExitData->callback.Reset(isolate, callback);

    uv_async_init(uv_default_loop(), &jsExitData->request, execAsyncCall);
}

void processQtEvents(const v8::FunctionCallbackInfo<v8::Value>&) {
    QCoreApplication::processEvents();
}

// exposes C++-functions in node.js-Module _rssModule (called CppDemoModule in node.js)
// format is NODE_SET_METHOD(exports, "functionNameInJS", functionPointerInC++).
// From what we understand, object member functions can not be exposed to node.js like this.
void onRegisterModule(v8::Local<v8::Object> exports, v8::Local<v8::Value>, v8::Local<v8::Context>, void * /*data*/) {
    NODE_SET_METHOD(exports, "cppLog", RssFeed::cppLog);
    NODE_SET_METHOD(exports, "registerExitFunc", registerExitFunc);
    NODE_SET_METHOD(exports, "clearFeed", RssFeed::clearFeed);
    NODE_SET_METHOD(exports, "redrawGUI", RssFeed::redrawGUI);
    NODE_SET_METHOD(exports, "processQtEvents", processQtEvents);
}

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

    QObject::connect(&engine, &QQmlEngine::quit, [](){ uv_async_send(&jsExitData->request); });

	std::string start_string("..\\..\\source\\demo-app\\rss_feed.js");
	int nodeArgc = 2;
	auto c = start_string.c_str();
	char *nodeArgv[2];
	nodeArgv[0] = argv[0];
	nodeArgv[1] = const_cast<char *>(c);

    node::Start(nodeArgc, nodeArgv);
}
