var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function(req, res) {
	res.sendFile(__dirname + '/index.html');
});

io.on('connection', function(socket) {
	console.log('a user connected');

	socket.on('getSize', size => {
		console.log('Size', size);

		io.emit('setSize', { inStock: size });
	});

	socket.on('reset', message => {
		io.emit('reset', message);
	});

	socket.on('connection', data => {
		console.log('connection', data);
	});

	socket.on('message', data => {
		console.log('message', data);
	});

	socket.on('atime', data => {
		console.log('atime', data);
	});
});

http.listen(4000, function() {
	console.log('listening on *:4000');
});
