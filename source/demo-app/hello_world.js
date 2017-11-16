console.log("Hello World from embedded JS!")

var cppDemoModule = process.binding('CppDemoModule')

var FeedParser = require('feedparser');
var request = require('request'); // for fetching the feed
 
var req = request('http://feeds.bbci.co.uk/news/world/rss.xml')
var feedparser = new FeedParser([]);
 
req.on('error', function (error) {
  // handle any request errors
});
 
req.on('response', function (res) {
  var stream = this; // `this` is `req`, which is a stream
 
  if (res.statusCode !== 200) {
    this.emit('error', new Error('Bad status code'));
  }
  else {
    stream.pipe(feedparser);
  }
});
 
feedparser.on('error', function (error) {
  // always handle errors
});
 
feedparser.on('readable', function () {
  // This is where the action is!
  var stream = this; // `this` is `feedparser`, which is a stream
  var meta = this.meta; // **NOTE** the "meta" is always available in the context of the feedparser instance
  var item;
 
  var itemString = '';
  while (item = stream.read()) {
      itemString = itemString + item['title'];
  }
  cppDemoModule.cppLog({itemTitle: itemString});
});
