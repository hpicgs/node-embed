#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent>
#include <QTimer>

#include <cpplocate/cpplocate.h>

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
    const std::string js_file = "data/node-qt-rss.js";
    const std::string data_path = cpplocate::locatePath(js_file);
    if (data_path.empty()) {
        std::cerr << "Could not find data path." << std::endl;
        return 1;
    }
    std::string js_path = data_path + "/" + js_file;

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

    const size_t argv0_len = std::strlen(argv[0]) + 1;
    const size_t argv1_len = js_path.size() + 1;
    const size_t total_size = argv0_len + argv1_len;

    char node_arg_string[total_size] = {0};
    std::strcpy(&node_arg_string[0], argv[0]);
    std::copy(js_path.begin(), js_path.end(), &node_arg_string[argv0_len]);

    char *node_argv[] = {&node_arg_string[0], &node_arg_string[argv0_len]};

    node::Start(2, node_argv);
}
