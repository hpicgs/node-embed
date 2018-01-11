#include "RssFeed.h"
#include <iostream>

RssFeed::RssFeed(QObject* parent)
    : QObject(parent)
{
    connect(&m_refreshTimer, &QTimer::timeout, this, &RssFeed::refreshFeed);

    QTimer::singleShot(200, this, &RssFeed::refreshFeed);
}

void RssFeed::refreshFeed() {
    v8::Isolate * isolate = v8::Isolate::GetCurrent();
    m_entries.clear();
    node::lib::call(node::lib::GetRootObject(),
                    "emitRequest",
                    v8::String::NewFromUtf8(isolate, "http://feeds.bbci.co.uk/news/world/rss.xml"),
                    v8::Function::New(isolate, [this](const v8::FunctionCallbackInfo<v8::Value> & info){ addEntry(info[0]); }),
                    v8::Function::New(isolate, [this](const v8::FunctionCallbackInfo<v8::Value> & info){ emit entriesChanged(); } ));
}

void RssFeed::addEntry(v8::Local<v8::Value> entry) {
    v8::String::Utf8Value v8String(entry);
    std::string stdString(*v8String);
    m_entries << QString::fromStdString(stdString);
    // shorter, but less informative:
    // m_entries << QString::fromLatin1(*v8String);
}
