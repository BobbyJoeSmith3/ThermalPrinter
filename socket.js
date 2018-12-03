var text; // variable for the text div to be created
var socket = new WebSocket('ws://localhost:8081');

function setup() {
  // event listeners for socket connection
  socket.onopen = openSocket;
  socket.onmessage = showData;

  // make new div a position it at 10, 10
  text = createDiv('Sensor reading: ');
  text.position(10,10);
}

// openSocket and showData callback functions for webSocket's event listeners
function openSocket() {
  text.html('Socket open');
  socket.send('Hello server');
}
// changes text inside the div and moves its horizontal position
// whenever new data arrives from server, this function will be called automatically, so there is no need for a draw() function
function showData(result) {
  // when the server returns show the result in the div:
  text.html(result.data);
  // xPos = int(result.data); // convert result to an integer
  // text.position(xPos, 10); // position the text
  // socket.send('a'); // send a byte to get Arduino to send new data
  if (result.data == "S\r") {
    sayHello();
  }
}


/*
NOTE: when p5.js returns the result of a request, it returns the string as an array with one element. If planning on manipulating that string with functions like parseInt(), split(), and other string handling functions, make sure to get the string out of the array first. Example of spliting the array:
function showData(result) {
  var resultString = result.data;
  text.html("Sensor reading:" + resultString);
  // split it:
  var numbers = split(resultString, ",");
  // use the numbers:
  text.position(int(numbers[0]), int(numbers[1]));
  text.style("font-size", int(numbers[2]) + "%");
}
*/
