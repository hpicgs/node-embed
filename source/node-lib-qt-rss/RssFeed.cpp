#include "RssFeed.h"

#include <iostream>
#include <QGuiApplication>
#include "node_lib.h"

RssFeed* RssFeed::instance = nullptr;

RssFeed::RssFeed(QObject* parent)
  : QObject(parent)
{

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

void RssFeed::redrawGUI(const v8::FunctionCallbackInfo<v8::Value>& args) {
  emit getInstance().entriesChanged();
}

void RssFeed::refreshFeed() {
  // invoke the embedded JavaScript in order to fetch new RSS feeds:
  node::Evaluate("emitRequest()");

  // wait for the embedded JavaScript to finish its execution
  // meanwhile, process any QT events
  node::RunEventLoop([](){ QGuiApplication::processEvents(); });
}

void RssFeed::addFeedItem(const v8::FunctionCallbackInfo<v8::Value>& args) {
  // check, whether this method was called as expected
  // therefore, we need to make sure, that the first argument exists
  // and that it is an object
  v8::Isolate* isolate = args.GetIsolate();
  if (args.Length() < 1 || !args[0]->IsObject()) {
    isolate->ThrowException(v8::Exception::TypeError(
                              v8::String::NewFromUtf8(isolate, "Error: One object expected")));
    return;
  }

  v8::Local<v8::Context> context = isolate->GetCurrentContext();
  v8::Local<v8::Object> obj = args[0]->ToObject(context).ToLocalChecked();

  // we want to get all properties of our input object:
  v8::Local<v8::Array> props = obj->GetOwnPropertyNames(context).ToLocalChecked();

  for (int i = 0, l = props->Length(); i < l; i++) {
    v8::Local<v8::Value> localKey = props->Get(i);
    v8::Local<v8::Value> localVal = obj->Get(context, localKey).ToLocalChecked();
    std::string key = *v8::String::Utf8Value(localKey);
    std::string val = *v8::String::Utf8Value(localVal);

    // append the RSS feed body to the list of RSS feeds:
    getInstance().entries << QString::fromStdString(val);
  }
}
