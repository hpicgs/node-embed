var cppDemoModule = process.binding('CppDemoModule');

var FeedParser = require('feedparser');
var request = require('request'); // for fetching the feed

var killMe = function () {
    process.exit();
}

cppDemoModule.registerExitFunc(killMe);

var emitRequest = function () {
  console.log("Refreshing feeds...")
  var feedparser = new FeedParser([]);  
  var req = request('http://feeds.bbci.co.uk/news/world/rss.xml')

  req.on('error', function (error) {
    // handle any request errors
  });

  req.on('response', function (res) {
    var stream = this; // `this` is `req`, which is a stream
  
    if (res.statusCode !== 200) {
      this.emit('error', new Error('Bad status code'));
    }
    else {
      cppDemoModule.clearFeed();
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
        itemString = itemString + item['title'] + '\n' + item['description'];
    }
    cppDemoModule.cppLog({itemTitle: itemString});
  });

  feedparser.on('end', function (){
    cppDemoModule.redrawGUI();
    setTimeout(emitRequest, 3000);
  });
}

emitRequest();

function processQtEvents() {
    cppDemoModule.processQtEvents()
    setTimeout(processQtEvents, 20);
}

processQtEvents();
