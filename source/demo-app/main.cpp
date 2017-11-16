#include <iostream>

#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QtConcurrent>

#include "node.h"

#include "RssFeed.h"

// Logging function for objects
void cppLog(const v8::FunctionCallbackInfo<v8::Value>& args)
{
  v8::Isolate* isolate = args.GetIsolate();

  if(args.Length() < 1 || !args[0]->IsObject()) {
    isolate->ThrowException(v8::Exception::TypeError(
    v8::String::NewFromUtf8(isolate, "Error: One object expected")));
    return;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Object> obj = args[0]->ToObject(context).ToLocalChecked();
  v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

  for(int i = 0, l = props->Length(); i < l; i++) {
    v8::Local<v8::Value> localKey = props->Get(i);
    v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
    std::string key = *v8::String::Utf8Value(localKey);
    std::string val = *v8::String::Utf8Value(localVal);
    std::cout << key << ":" << val << std::endl;
  }
}

void onRegisterModule(v8::Local<v8::Object> exports, v8::Local<v8::Value>, v8::Local<v8::Context>, void * data)
{
    std::cout << "onRegisterModule() for CppDemoModule was called"  << std::endl;
    NODE_SET_METHOD(exports, "cppLog", cppLog);
}


int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    RssFeed rssFeed;
    // to be able to access the public slots of the RssFeed instance
    // we inject a pointer to it in the QML context:
    engine.rootContext()->setContextProperty("rssFeed", &rssFeed);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty()) {
        qWarning() << "Error while loading QML file";
        return -1;
    }

    // Create node module
    static node::node_module _module =
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
    node_module_register(&_module);

    std::cout << "Hello World from embedding C++!" << std::endl;
    QtConcurrent::run([argc, argv](){ node::Start(argc, argv); });

    return app.exec();
}
