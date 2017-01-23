var express = require('express');
var app = express();
var cache = require('memory-cache');
const instagramAnalytics = require('instagram-analytics');
const cacheTime = 1000 * 15; // 15 sec

app.get('/followers/:ig_handle', function(req, res) {
  get(req.params["ig_handle"], function(data) {
    res.send(data.followers.toString());
  });
});

app.get('/likes/:ig_handle', function(req, res) {
  get(req.params["ig_handle"], function(data) {
    res.send(data.likes.toString());
  });
});

app.get('/comments/:ig_handle', function(req, res) {
  get(req.params["ig_handle"], function(data) {
    res.send(data.comments.toString());
  });
});

function get(handle, completion) {
  var cachedValue = cache.get(handle);
  if (cachedValue) {
    console.log('Found follower stats for ' + handle + ' in cache');
    completion(cachedValue);
  } else {
    console.log('Will query ' + handle + ' for stats');

    // The "count" option refers to the number of posts to analyze (default is 20)
    instagramAnalytics.users([handle], {count : 60}).then(stats => {
      cache.put(handle, stats[0], cacheTime, function(key, value) {
        console.log(key + ' was cleared from cache (had value ' + value + ')');
      });
      completion(stats[0]);
    });
  }
}

app.listen(process.env.PORT || 3000, function () {
  console.log('App and running');
});
