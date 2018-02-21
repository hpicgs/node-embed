#ifndef RSSFEED_H
#define RSSFEED_H

#include <QObject>
#include "node.h"

/**
 * @brief The RssFeed class retrieves an RSS feed from the Internet and
 * provides its entries.
 */
class RssFeed : public QObject {

  Q_OBJECT

  Q_PROPERTY(QStringList entries READ getEntries NOTIFY entriesChanged)

private:
  /**
   * @brief Creates a new RssFeed with a given QObject as its parent.
   *
   * @param parent The parent object.
   */
  explicit RssFeed(QObject* parent=nullptr);

public:
  /**
   * @brief Returns the singleton instance for this class.
   *
   * @return The singlton instance for this class.
   */
  static RssFeed& getInstance();

  /**
   * @brief This method is called from the embedded JavaScript.
   * It is used for deleting all items from this RSS feed.
   *
   * @param args The arguments passed from the embedded JavaScript.
   * Hint: This method does not expect any arguments.
   */
  static void clearFeed(const v8::FunctionCallbackInfo<v8::Value>& args);

  /**
   * @brief This method is called from the embedded JavaScript.
   * It is used to add an entry to this RSS feed.
   *
   * @param args The arguments passed from the embedded JavaScript.
   * Hint: This method expects an object, which contains the RSS feed item.
   */
  static void addFeedItem(const v8::FunctionCallbackInfo<v8::Value>& args);

  /**
   * @brief This method is called from the embedded JavaScript.
   * It is used to refresh the GUI, after all retrieved feed items have been
   * appended.
   *
   * @param args The arguments passed from the embedded JavaScript.
   * Hint: This method does not expect any arguments.
   */
  static void redrawGUI(const v8::FunctionCallbackInfo<v8::Value>& args);

private:
  static RssFeed* instance;   /*!< The singleton instance for this class. */
  QStringList entries;        /*!< The list of RSS feeds to display. */

signals:
  void entriesChanged();      /*!< Emitted after entries were added or removed. */

public slots:
  /**
   * @brief getEntries returns the entries of the RSS feed
   *
   * @return a list of text entries
   */
  QStringList getEntries() const;
};

#endif // RSSFEED_H
