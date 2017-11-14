#include "RssFeed.h"

RssFeed::RssFeed(QObject* parent)
    : QObject(parent)
{

}

QStringList RssFeed::getEntries() const {
    QStringList entries;
    entries << "Entry 1" << "Entry 2" << "Entry 3";
    return entries;
}
