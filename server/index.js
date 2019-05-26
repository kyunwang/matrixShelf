// // Super simple web socket server for prototyping
// const port = 4000;

// // const app = require('express')();
// const WebSocket = require('ws');

// const wss = new WebSocket.Server({
// 	port,
// 	perMessageDeflate: {
// 		zlibDeflateOptions: {
// 			// See zlib defaults.
// 			chunkSize: 1024,
// 			memLevel: 7,
// 			level: 3,
// 		},
// 		zlibInflateOptions: {
// 			chunkSize: 10 * 1024,
// 		},
// 		// Other options settable:
// 		clientNoContextTakeover: true, // Defaults to negotiated value.
// 		serverNoContextTakeover: true, // Defaults to negotiated value.
// 		serverMaxWindowBits: 10, // Defaults to negotiated value.
// 		// Below options specified as default values.
// 		concurrencyLimit: 10, // Limits zlib concurrency for perf.
// 		threshold: 1024, // Size (in bytes) below which messages
// 		// should not be compressed.
// 	},
// });

// wss.on('connection', function connection(ws) {
// 	console.log('Connected');

// 	ws.on('message', function incoming(message) {
// 		try {
// 			const json = JSON.parse(message);
// 			console.log('received: %s', message);

// 			sendToShelves(json.value);
// 		} catch (err) {
// 			console.log('received non JSON: %s', message);
// 		}
// 	});

// 	function sendToShelves(value) {
// 		ws.send(value);
// 	}
// });

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

		io.emit('setSize', { message: size });
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
