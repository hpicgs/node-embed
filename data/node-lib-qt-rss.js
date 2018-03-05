var FeedParser = require('feedparser');
var request = require('request');

var feedparser = new FeedParser();

feedparser.on('readable', function () {
  var item = this.read();

  if (item && item['title']) {
    cppQtGui.addFeedItem(item);
  }
});

function loadBBCFeed() {
  var req = request('http://feeds.bbci.co.uk/news/world/rss.xml');

  req.on('response', function (res) {
    cppQtGui.clearFeed();
    this.pipe(feedparser);
  });
}
