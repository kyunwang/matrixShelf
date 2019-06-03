import './main.scss';

// const ws = new WebSocket('ws://localhost:4000/');
import io from 'socket.io-client';
const socket = io('http://localhost:4000');
// const socket = io('http://145.109.169.207:4000');
// const socket = io('70b556e8.ngrok.io:80');
// const socket = io('http://192.168.1.12:4000');

function init() {
	readyEventListiners();
	readyWebsockets();
}

function readyEventListiners() {
	const button = document.querySelector('#submit-btn');
	const sizeValue = document.querySelector('#shoe-size');
	const resetBtn = document.querySelector('#reset');

	button.addEventListener('click', event => {
		const value = sizeValue.value;

		if (value) {
			socket.emit('getSize', value);
		}
		event.preventDefault();
	});

	resetBtn.addEventListener('click', event => {
		socket.emit('reset', { message: true });
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
