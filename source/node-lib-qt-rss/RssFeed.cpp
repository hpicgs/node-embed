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

void RssFeed::redrawGUI() {
  emit getInstance().entriesChanged();
}

void RssFeed::refreshFeed() {
  // Invoke the embedded JavaScript in order to fetch the RSS feed
  node::Evaluate("loadBBCFeed()");

  // Wait for the embedded JavaScript to finish its execution.
  // Meanwhile, process any Qt events
  node::RunEventLoop([](){ QGuiApplication::processEvents(); });
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

  v8::Local<v8::String> v8Title          = item->Get(context, v8TitleKey).ToLocalChecked();
  v8::Local<v8::String> v8Description    = item->Get(context, v8DescriptionKey).ToLocalChecked();

  std::string title                      = *v8::String::Utf8Value(v8Title);
  std::string description                = *v8::String::Utf8Value(v8Description);

  getInstance().entries << QString::fromStdString(title + "\n" + description);
  RssFeed::redrawGUI();
}
