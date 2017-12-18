#include "RssFeed.h"
#include <iostream>

RssFeed::RssFeed(QObject* parent)
    : QObject(parent)
{
    node::lib::call("getFeed", 'http://feeds.bbci.co.uk/news/world/rss.xml',
                    [this](v8::Local<v8::Value> entry){ addEntry(entry); });
}

void RssFeed::addEntry(v8::Local<v8::Value> entry) {
    // TODO: convert entry to QString
    getInstance().entries << QString::fromStdString(val);
    emit entriesChanged();

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
