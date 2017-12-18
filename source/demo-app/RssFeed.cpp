#include "RssFeed.h"
#include <iostream>

RssFeed::RssFeed(QObject* parent)
    : QObject(parent)
{
    connect(&m_refreshTimer, &QTimer::timeout, this, &RssFeed::refreshFeed);

    QTimer::singleShot(200, &RssFeed::refreshFeed);
}

void RssFeed::refreshFeed() {
    m_entries.clear();
    node::lib::call("emitRequest",
                    'http://feeds.bbci.co.uk/news/world/rss.xml',
                    [this](v8::Local<v8::Value> entry){ addEntry(entry); },
                    [this](){ emit entriesChanged(); } );
}

void RssFeed::addEntry(v8::Local<v8::Value> entry) {
    v8::String::Utf8Value v8String(entry);
    std::string stdString(*v8String);
    m_entries << QString::fromStdString(stdString);
    // shorter, but less informative:
    // m_entries << QString::fromLatin1(*v8String);
}
