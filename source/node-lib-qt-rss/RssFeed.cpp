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
  entries << QString::fromStdString("No Entries\n---");
}

RssFeed::~RssFeed() {

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

}

void RssFeed::addFeedItem(const v8::FunctionCallbackInfo<v8::Value>& args) {
  /*v8::Isolate* isolate = args.GetIsolate();

  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  // Idea:
  // title = item["title"]
  // description = item["description"]

  // TODO

  v8::Local<v8::String> v8TitleKey       = v8::String::NewFromUtf8(isolate, "title");
  v8::Local<v8::String> v8DescriptionKey = v8::String::NewFromUtf8(isolate, "description");

  // TODO

  std::string title                      = *v8::String::Utf8Value(v8Title);
  std::string description                = *v8::String::Utf8Value(v8Description);

  getInstance().entries << QString::fromStdString(title + "\n" + description);
  RssFeed::redrawGUI();*/
}
