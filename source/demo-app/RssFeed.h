#ifndef RSSFEED_H
#define RSSFEED_H

#include <QObject>
#include "node.h"

/**
 * @brief The RssFeed class retrieves an RSS feed from the Internet and provides its entries.
 */
class RssFeed : public QObject {

    Q_OBJECT

    Q_PROPERTY(QStringList entries READ getEntries NOTIFY entriesChanged)

public:
    explicit RssFeed(QObject* parent=nullptr);

    void addEntry(v8::Local<v8::Value> entry);

private:
    QStringList entries;

signals:
    void entriesChanged();

public slots:
    /**
     * @brief getEntries returns the entries of the RSS feed
     * @return a list of text entries
     */
    QStringList getEntries() const;
};

#endif // RSSFEED_H
