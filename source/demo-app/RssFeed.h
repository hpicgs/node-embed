#ifndef RSSFEED_H
#define RSSFEED_H

#include <QObject>
#include <QTimer>

#include "v8.h"
#include "node.h"
#include "node_lib.h"

/**
 * @brief The RssFeed class retrieves an RSS feed from the Internet and provides its entries.
 */
class RssFeed : public QObject {

    Q_OBJECT

    Q_PROPERTY(QStringList entries READ getEntries NOTIFY entriesChanged)

public:
    explicit RssFeed(QObject* parent=nullptr);

    /**
     * @brief refreshFeed starts retrieving the updated feed entries using JS
     */
    void refreshFeed();

    /**
     * @brief addEntry adds an entry to this feed
     * @param entry v8 string object containing the text of the entry
     */
    void addEntry(v8::Local<v8::Value> entry);

signals:
    /**
     * @brief entriesChanged is emitted when entries changed
     */
    void entriesChanged();

public slots:
    /**
     * @brief getEntries returns the entries of the RSS feed
     * @return a list of text entries
     */
    QStringList getEntries() const { return m_entries; }

private:
    QStringList m_entries;
    QTimer m_refreshTimer;
};

#endif // RSSFEED_H
