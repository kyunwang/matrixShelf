// const ws = new WebSocket('ws://localhost:4000/');
import io from 'socket.io-client';
// const socket = io('http://localhost:4000');
const socket = io('http://192.168.1.12:4000');

function init() {
	readyEventListiners();
	readyWebsockets();
}

function readyEventListiners() {
	const button = document.querySelector('#submit-btn');
	const sizeValue = document.querySelector('#shoe-size');

	button.addEventListener('click', event => {
		const value = sizeValue.value;

		if (value) {
			// const json = JSON.stringify({
			// 	type: 'input',
			// 	value,
			// });

			// ws.send(json);
			socket.emit('getSize', value);
		}
		event.preventDefault();
	});
}

// function readyWebsockets() {
// 	ws.onopen = event => {
// 		console.log('Connected to socket server');
// 	};

// 	ws.onmessage = event => {
// 		console.log('dsad');
// 	};
// }

function readyWebsockets() {
	socket.on('connect', function() {
		console.log('Connected');
	});
	socket.on('event', function(data) {
		console.log(data);
	});
	socket.on('disconnect', function() {
		console.log('Disconnected');
	});
}

init();
