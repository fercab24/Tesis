var ZongJi = require('./node_modules/zongji/');

var zongji = new ZongJi({
  host     : 'localhost',
  user     : 'root',
  password : 'root',
  // debug: true
});

zongji.on('binlog', function(evt) {
  evt.dump();
});

zongji.start({
  //includeEvents: ['tablemap', 'writerows', 'updaterows', 'deleterows']
  includeEvents: ['tablemap','writerows','updaterows']
});

process.on('SIGINT', function() {
  console.log('Got SIGINT.');
  zongji.stop();
  process.exit();
});

