#include "RssFeed.h"

#include <iostream>
#include <QGuiApplication>
#include <QDebug>

#include <cpplocate/cpplocate.h>

#include "node_lib.h"

RssFeed* RssFeed::instance = nullptr;

RssFeed::RssFeed(QObject* parent)
  : QObject(parent)
{
  // Locate the JavaScript file we want to load:
  const std::string js_file = "data/node-lib-qt-rss.js";
  const std::string data_path = cpplocate::locatePath(js_file);
  if (data_path.empty()) {
    qWarning() << "Could not find data path.";
    throw std::exception();
  }
  const std::string js_path = data_path + "/" + js_file;

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
}

RssFeed::~RssFeed() {
  // After we are done, deinitialize the Node.js engine:
  node::Deinitialize();
}

RssFeed& RssFeed::getInstance(){
  if (instance == nullptr){
    instance = new RssFeed();
  }
  return *instance;
}

QStringList RssFeed::getEntries() const {
  return entries;
}

void RssFeed::clearFeed(const v8::FunctionCallbackInfo<v8::Value>& args) {
  getInstance().entries.clear();
}

void RssFeed::redrawGUI() {
  emit getInstance().entriesChanged();
}

void RssFeed::refreshFeed() {
  // Invoke the embedded JavaScript in order to fetch the RSS feed
  node::Evaluate("loadBBCFeed()");

  // Wait for the embedded JavaScript to finish its execution.
  // Meanwhile, process any Qt events
  node::RunEventLoop([](){ QGuiApplication::processEvents(); });

  // Let's send a system notification that we are ready.
  // We can either use node::Evaluate(), which pollutes the global namespace
  // but is easier to read.
  // Or we can use V8 objects to include the module and call the notify
  // function directly on the object.
  // Both methods are shown below.

  // node::Evaluate("notifier = require('node-notifier');");
  // node::Evaluate("notifier.notify('RSS Reader ready.');");

  auto notifier = node::IncludeModule("node-notifier").ToLocalChecked();
  auto isolate = node::internal::isolate();
  node::Call(notifier, "notify", {v8::String::NewFromUtf8(isolate, "RSS feed updated")});
}

void RssFeed::addFeedItem(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() != 1 || !args[0]->IsObject()) {
    isolate->ThrowException(v8::Exception::TypeError(
                              v8::String::NewFromUtf8(isolate, "Error: One object expected")));
    return;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  v8::Local<v8::Object> item = args[0]->ToObject(context).ToLocalChecked();

  v8::Local<v8::String> v8TitleKey       = v8::String::NewFromUtf8(isolate, "title");
  v8::Local<v8::String> v8DescriptionKey = v8::String::NewFromUtf8(isolate, "description");

  v8::Local<v8::Value> v8Title          = item->Get(context, v8TitleKey).ToLocalChecked();
  v8::Local<v8::Value> v8Description    = item->Get(context, v8DescriptionKey).ToLocalChecked();

  std::string title                      = *v8::String::Utf8Value(v8Title);
  std::string description                = *v8::String::Utf8Value(v8Description);

  getInstance().entries << QString::fromStdString(title + "\n" + description);
  RssFeed::redrawGUI();
}
