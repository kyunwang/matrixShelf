var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.get('/', function(req, res) {
	res.sendFile(__dirname + '/index.html');
});

let shelfIndex = 0;

io.on('connection', function(socket) {
	console.log('a user connected');

	socket.on('getSize', size => {
		console.log('Size', size);

		io.emit('setSize', { inStock: size });
	});

	socket.on('reset', message => {
		console.log('Resetting: ');

		io.emit('reset', message);
	});

	socket.on('hasReset', message => {
		console.log('Has reset');
	});

	socket.on('connected', data => {
		shelfIndex++;

		const isInStore = shelfIndex % 3; // 0, 1, 2

		console.log('socket connected', data);
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
