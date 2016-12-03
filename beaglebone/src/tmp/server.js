var express = require('express')
var app = express()
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function (req, res) {
  res.send('Hello World!')
})

io.on('connection', function(socket){
    console.log('a user connected');
    socket.on('disconnect', function(){
        console.log('user disconnected');
    });

    socket.on('chat message', function(msg){
        console.log(msg);
        io.emit('chat message', msg);
    });
});
app.use(express.static(__dirname + '/templates'));

app.use(express.static(__dirname + '/templates/assets'));

http.listen(5000, function () {
  console.log('Example app listening on port 5000!')
})


